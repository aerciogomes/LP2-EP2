#include "banheiro.h"
#include "Pessoa.h"
#include <iostream>
#include <mutex>
#include <time.h>
#include "windows.h"

std::mutex Pessoa::entrada;
std::mutex Pessoa::homem;
std::mutex Pessoa::mulher;
std::mutex Pessoa::mensagem;
std::mutex Pessoa::slept;
int tempo;
using namespace std;

Pessoa::Pessoa(int id,char sexo)
{
    this->id = id;
    this->sexo = sexo;
    srand(time(NULL));
}
int Pessoa::getId(){

    return id;
}
char Pessoa::getSexo(){
    return sexo;
}
void Pessoa::EntraBanheiro(){
    if(getSexo() == 'H'){
        /*Pessoa::mensagem.lock();
        std::cout << "HOMEM[" << getId() << "] CHEGOU" << std::endl;
        Pessoa::mensagem.unlock();*/

        Pessoa::entrada.lock();

        if((Banheiro::MULHERES_BANHEIRO > 0) || (Banheiro::CAPACIDADE == Banheiro::HOMENS_BANHEIRO) ||
           ((Banheiro::HOMENS_SEG >= Banheiro::LIMITE_SEG) && (Banheiro::MULHERES_FILA > 0))){

                Banheiro::HOMENS_FILA++;
                Pessoa::mensagem.lock();
                cout << "HOMEM[" << getId() << "] ENTROU NA FILA" << endl;
                Pessoa::mensagem.unlock();

                Pessoa::entrada.unlock();
                Pessoa::homem.lock();

        }
        //atualizando
        Banheiro::HOMENS_BANHEIRO++;
        Banheiro::HOMENS_SEG++;
        Banheiro::MULHERES_SEG = 0;
        Banheiro::UTILIZACAO_B++;
        Pessoa::mensagem.lock();
        cout << "UTILIZAÇÃO[" << Banheiro::UTILIZACAO_B << "]" <<endl;
        Pessoa::mensagem.unlock();
        //passagem de bastão
        if(Banheiro::HOMENS_BANHEIRO < Banheiro::CAPACIDADE){
            if(Banheiro::HOMENS_SEG < Banheiro::LIMITE_SEG && Banheiro::HOMENS_FILA > 0){
                Banheiro::HOMENS_FILA--;
                Pessoa::homem.unlock();
            }else{

                Pessoa::entrada.unlock();
                Pessoa::mensagem.lock();
                cout << "OPA"<< endl;
                Pessoa::mensagem.unlock();
                //entrou no banheiro
            }
        }else{

            Pessoa::entrada.unlock();
        }
        Pessoa::mensagem.lock();
        std::cout << "HOMEM[" << getId() << "] ENTROU NO BANHEIRO" << std::endl;
        Pessoa::mensagem.unlock();

    }
    if(getSexo() == 'M'){
        /*Pessoa::mensagem.lock();
        std::cout << "MULHER[" << getId() << "] CHEGOU" << std::endl;
        Pessoa::mensagem.unlock();*/

        Pessoa::entrada.lock();
        //volta pra fila
        if((Banheiro::HOMENS_BANHEIRO > 0) || (Banheiro::MULHERES_BANHEIRO == Banheiro::CAPACIDADE) ||
           ((Banheiro::MULHERES_SEG >= Banheiro::LIMITE_SEG) && (Banheiro::HOMENS_FILA > 0)) ){

            Banheiro::MULHERES_FILA++;

            Pessoa::mensagem.lock();
            std::cout << "MULHER[" << getId() << "] ENTROU NA FILA" << std::endl;
            Pessoa::mensagem.unlock();

            Pessoa::entrada.unlock();
            Pessoa::mulher.lock();
        }
        //atualiza os contadores
        if(Banheiro::UTILIZACAO_B < Banheiro::MAX_UTILIZACAO){
        Banheiro::MULHERES_SEG++;
        Banheiro::HOMENS_SEG = 0;
        Banheiro::MULHERES_BANHEIRO++;
        Banheiro::UTILIZACAO_B++;

        Pessoa::mensagem.lock();
        cout << "UTILIZAÇÃO[" << Banheiro::UTILIZACAO_B << "]" <<endl;
        Pessoa::mensagem.unlock();

        /*Pessoa::mensagem.lock();
        cout << "MULHER SEGUIDA "<< Banheiro::MULHER_SEG << endl;
        Pessoa::mensagem.unlock();*/

        //passagem de bastão
        if(Banheiro::MULHERES_BANHEIRO < Banheiro::CAPACIDADE && Banheiro::MULHERES_SEG < Banheiro::LIMITE_SEG && Banheiro::MULHERES_FILA > 0){
                Banheiro::MULHERES_FILA--;
                Pessoa::mulher.unlock();
                Pessoa::mensagem.lock();
                cout << "OPA2" <<endl;
                Pessoa::mensagem.unlock();
        }
        else
            Pessoa::entrada.unlock();
            //entrou no banheiro
            Pessoa::mensagem.lock();
            std::cout << "MULHER[" << getId() << "] ENTROU NO BANHEIRO" << std::endl;
            Pessoa::mensagem.unlock();

        }else{
            Pessoa::mensagem.lock();
        }
    }

}
void Pessoa::SaiBanheiro(){
     if(getSexo() == 'H'){
        Pessoa::entrada.lock();

        Banheiro::HOMENS_BANHEIRO--;

        Pessoa::mensagem.lock();
        std::cout << "HOMEM[" << getId() << "] SAIU DO BANHEIRO" << std::endl;
        Pessoa::mensagem.unlock();

        //(signal)
        if(Banheiro::MULHERES_FILA > 0 && Banheiro::HOMENS_BANHEIRO == 0){
                Banheiro::MULHERES_FILA--;
                Pessoa::mulher.unlock();
        }
        else {
            if(Banheiro::HOMENS_FILA > 0 && Banheiro::HOMENS_SEG < Banheiro::LIMITE_SEG){
                Banheiro::HOMENS_FILA--;
                Pessoa::homem.unlock();
            }
            else
                Pessoa::entrada.unlock();
        }
    }

    if(getSexo() == 'M'){
        Pessoa::entrada.lock();

        Pessoa::mensagem.lock();
        std::cout << "MULHER[" << getId() << "] SAIU DO BANHEIRO" << std::endl;
        Pessoa::mensagem.unlock();

        Banheiro::MULHERES_BANHEIRO--;

        //signal
        if(Banheiro::MULHERES_BANHEIRO == 0 && Banheiro::HOMENS_FILA > 0){
                Banheiro::HOMENS_FILA--;
                Pessoa::homem.unlock();
            }
        else {
            if(Banheiro::MULHERES_FILA > 0 && Banheiro::MULHERES_SEG < Banheiro::LIMITE_SEG){
                Banheiro::MULHERES_FILA--;
                Pessoa::mulher.unlock();
            }
            else
                Pessoa::entrada.unlock();
        }
    }

}
void Pessoa::darVolta(){

    Pessoa::slept.lock();
    tempo = ((rand() % 300) + 5);
    Sleep(tempo);
    cout << "TEMPO = " << tempo << endl;
    Pessoa::slept.unlock();

    Pessoa::mensagem.lock();
    cout << "Deu uma volta" <<endl;
    Pessoa::mensagem.unlock();

}

void Pessoa::Run(){
    while(Banheiro::UTILIZACAO_B < Banheiro::MAX_UTILIZACAO){
            EntraBanheiro();
            if(Banheiro::UTILIZACAO_B >= Banheiro::MAX_UTILIZACAO){
                break;
            }
            SaiBanheiro();

            darVolta();


    }

}
