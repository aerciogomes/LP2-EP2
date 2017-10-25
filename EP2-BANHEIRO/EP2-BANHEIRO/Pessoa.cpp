#include "banheiro.h"
#include "Pessoa.h"
#include <iostream>
#include <mutex>
#include <time.h>
#include "windows.h"
#include <chrono>
#include <thread>
#include <conio.h>

std::mutex Pessoa::entrada;
std::mutex Pessoa::homem;
std::mutex Pessoa::mulher;
std::mutex Pessoa::mensagem;
std::mutex Pessoa::slept;
int tempo = ((rand() % 300) + 5);;

using namespace std;

void Pessoa::Tempo(int temp)
{
    chrono::seconds time(temp);
    this_thread::sleep_for(time);
}

void Pessoa::EsperaNoBanheiro()
{
    Tempo( rand()% 3 );
}
void Pessoa::TempoDeVolta()
{
    Tempo( rand() % 5 );
}

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
        Pessoa::mensagem.lock();
        std::cout << "HOMEM[" << getId() << "] INICIOU" << std::endl << endl;
        Pessoa::mensagem.unlock();

        Pessoa::entrada.lock();

        if(Banheiro::UTILIZACAO_B < Banheiro::MAX_UTILIZACAO){
        if((Banheiro::MULHERES_BANHEIRO > 0) || (Banheiro::CAPACIDADE == Banheiro::HOMENS_BANHEIRO) ||
           ((Banheiro::HOMENS_SEG >= Banheiro::LIMITE_SEG) && (Banheiro::MULHERES_FILA > 0) ) ){

                Banheiro::HOMENS_FILA++;
                Pessoa::mensagem.lock();
                cout << "HOMEM[" << getId() << "] ENTROU NA FILA" << endl;
                cout << "HOMENS NA FILA : " << Banheiro::HOMENS_FILA << endl;
                cout << "HOMENS NO BANHEIRO: " << Banheiro::HOMENS_BANHEIRO << endl;
                cout << "HOMENS CONSECUTIVOS: " << Banheiro::HOMENS_SEG << endl;
                cout << "MULHERES NA FILA: " << Banheiro::MULHERES_FILA << endl;
                cout << "MULHERES NO BANHEIRO: " << Banheiro::MULHERES_BANHEIRO << endl;
                cout << "MULHERES CONSECUTIVAS: " << Banheiro::MULHERES_SEG << endl << endl;
                Pessoa::mensagem.unlock();

                Pessoa::entrada.unlock();
                Pessoa::homem.lock();

        }
        //atualizando
        if(Banheiro::UTILIZACAO_B < Banheiro::MAX_UTILIZACAO){
        Banheiro::HOMENS_BANHEIRO++;
        Banheiro::HOMENS_SEG++;
        Banheiro::MULHERES_SEG = 0;
        Banheiro::UTILIZACAO_B++;
        Pessoa::mensagem.lock();
        cout << "HOMEN [" << getId() << "] ENTROU BANHEIRO" << endl;
        cout << "HOMENS NA FILA: " << Banheiro::HOMENS_FILA << endl;
        cout << "HOMENS NO BANHEIRO: " << Banheiro::HOMENS_BANHEIRO << endl;
        cout << "HOMENS CONSECUTIVOS: " << Banheiro::HOMENS_SEG << endl;
        cout << "MULHERES NA FILA: " << Banheiro::MULHERES_FILA << endl;
        cout << "MULHERES NO BANHEIRO: " << Banheiro::MULHERES_BANHEIRO << endl;
        cout << "MULHERES CONSECUTIVAS: " << Banheiro::MULHERES_SEG << endl;
        cout << "UTILIZACAO: " << Banheiro::UTILIZACAO_B << endl;
        cout << endl;
        cout << endl;
        Pessoa::mensagem.unlock();

        /*Pessoa::mensagem.lock();
        cout << "HOMEM SEGUIDOS = " << Banheiro::HOMENS_SEG << "" << endl;
        Pessoa::mensagem.unlock();*/

        //passagem de bastão - 1º SIGNAL
        if(Banheiro::HOMENS_BANHEIRO < Banheiro::CAPACIDADE){//VERIFICA SE CABE MAIS HOMENS NO BANHEIRO
            if(Banheiro::HOMENS_SEG < Banheiro::LIMITE_SEG && Banheiro::HOMENS_FILA > 0){// VERIFICA SE ATENDE AS CONDICOES PARA ENTRAR MAIS HOMENS(HOMENS SEGUIDOS)
                Banheiro::HOMENS_FILA--;
                Pessoa::homem.unlock();

            }else{//LIBERA A ENTRADA NA FILA DE OUTRAS PESSOAS

                Pessoa::entrada.unlock();
                /*Pessoa::mensagem.lock();
                cout << "OPA"<< endl;
                Pessoa::mensagem.unlock();*/
            }
        }else{//LIBERA A ENTRADA DE OUTRAS PESSOAS

            Pessoa::entrada.unlock();
        }

        Pessoa::EsperaNoBanheiro();
        /*Pessoa::mensagem.lock();
        std::cout << "HOMEM[" << getId() << "] ENTROU NO BANHEIRO" << std::endl;
        Pessoa::mensagem.unlock();

        Pessoa::mensagem.lock();
        cout << "UTILIZACAO[" << Banheiro::UTILIZACAO_B << "]" <<endl;
        Pessoa::mensagem.unlock();*/

        /*Pessoa::mensagem.lock();
        cout << "HOMENS NA FILA: " << Banheiro::HOMENS_FILA << endl;
        cout << "HOMENS NO BANHEIRO: " << Banheiro::HOMENS_BANHEIRO << endl;
        cout << "HOMENS CONSECUTIVOS: " << Banheiro::HOMENS_SEG << endl;
        cout << "MULHERES NA FILA: " << Banheiro::MULHERES_FILA << endl;
        cout << "MULHERES NO BANHEIRO: " << Banheiro::MULHERES_BANHEIRO << endl;
        cout << "MULHERES CONSECUTIVAS: " << Banheiro::MULHERES_SEG << endl;
        cout << endl;
        cout << endl;
        Pessoa::mensagem.unlock();*/

    }
        }
    }
    if(getSexo() == 'M'){
        Pessoa::mensagem.lock();
        std::cout << "MULHER[" << getId() << "] INICIOU" << std::endl;
        Pessoa::mensagem.unlock();

        Pessoa::entrada.lock();

        if(Banheiro::UTILIZACAO_B < Banheiro::MAX_UTILIZACAO){// QUANDO O PROGRAMA NÃO ACABA
        //FAZ A MULHER TRAVAR(FICAR NA FILA)
        if((Banheiro::HOMENS_BANHEIRO > 0) || (Banheiro::MULHERES_BANHEIRO == Banheiro::CAPACIDADE) ||
           ((Banheiro::MULHERES_SEG >= Banheiro::LIMITE_SEG) && (Banheiro::HOMENS_FILA > 0) ) ){

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

        /*Pessoa::mensagem.lock();
        cout << "MULHER SEGUIDA "<< Banheiro::MULHERES_SEG << endl;
        Pessoa::mensagem.unlock();*/
        Pessoa::mensagem.lock();
        cout << "MULHER [" << getId() << "] ENTROU NO BANHEIRO" << endl;
        cout << "HOMENS NA FILA: " << Banheiro::HOMENS_FILA << endl;
        cout << "HOMENS NO BANHEIRO: " << Banheiro::HOMENS_BANHEIRO << endl;
        cout << "HOMENS CONSECUTIVOS: " << Banheiro::HOMENS_SEG << endl;
        cout << "MULHERES NA FILA: " << Banheiro::MULHERES_FILA << endl;
        cout << "MULHERES NO BANHEIRO: " << Banheiro::MULHERES_BANHEIRO << endl;
        cout << "MULHERES CONSECUTIVAS: " << Banheiro::MULHERES_SEG << endl;
        cout << "UTILIZACAO: " << Banheiro::UTILIZACAO_B << endl;
        cout << endl;
        cout << endl;
        Pessoa::mensagem.unlock();

        //passagem de bastão - 1º SIGNAL
        if(Banheiro::MULHERES_BANHEIRO < Banheiro::CAPACIDADE && Banheiro::MULHERES_SEG < Banheiro::LIMITE_SEG && Banheiro::MULHERES_FILA > 0){
                Banheiro::MULHERES_FILA--;
                Pessoa::mulher.unlock();
                /*Pessoa::mensagem.lock();
                cout << "OPA2" <<endl;
                Pessoa::mensagem.unlock();*/
        }
        else
            Pessoa::entrada.unlock();

        }else{
            Pessoa::entrada.unlock();
            //V(E) PARA LIBERAR PESSOAS TRAVADAS APÓS O NUMERO DE UTILIZAÇÕES ACABEREM
        }
        /*Pessoa::mensagem.lock();
        cout << "UTILIZACAO[" << Banheiro::UTILIZACAO_B << "]" <<endl;
        Pessoa::mensagem.unlock();*/

        /*Pessoa::mensagem.lock();
        cout << "HOMENS NA FILA: " << Banheiro::HOMENS_FILA << endl;
        cout << "HOMENS NO BANHEIRO: " << Banheiro::HOMENS_BANHEIRO << endl;
        cout << "HOMENS CONSECUTIVOS: " << Banheiro::HOMENS_SEG << endl;
        cout << "MULHERES NA FILA: " << Banheiro::MULHERES_FILA << endl;
        cout << "MULHERES NO BANHEIRO: " << Banheiro::MULHERES_BANHEIRO << endl;
        cout << "MULHERES CONSECUTIVAS: " << Banheiro::MULHERES_SEG << endl;
        cout << endl;
        cout << endl;
        Pessoa::mensagem.unlock();*/
    }else{
        Pessoa::entrada.unlock();
    }
    Pessoa::EsperaNoBanheiro();
    }

}

void Pessoa::SaiBanheiro(){
     if(getSexo() == 'H'){
        Pessoa::entrada.lock();

        Banheiro::HOMENS_BANHEIRO--;

        Pessoa::mensagem.lock();
        std::cout << "HOMEM[" << getId() << "] SAIU DO BANHEIRO" << std::endl;
        cout << "HOMENS NA FILA: " << Banheiro::HOMENS_FILA << endl;
        cout << "HOMENS NO BANHEIRO: " << Banheiro::HOMENS_BANHEIRO << endl;
        cout << "HOMENS CONSECUTIVOS: " << Banheiro::HOMENS_SEG << endl;
        cout << "MULHERES NA FILA: " << Banheiro::MULHERES_FILA << endl;
        cout << "MULHERES NO BANHEIRO: " << Banheiro::MULHERES_BANHEIRO << endl;
        cout << "MULHERES CONSECUTIVAS: " << Banheiro::MULHERES_SEG << endl << endl;
        Pessoa::mensagem.unlock();

        //(2º signal) = PASSAGEM DE BASTAO
        if((Banheiro::MULHERES_FILA > 0 && Banheiro::HOMENS_BANHEIRO == 0)){
                Banheiro::MULHERES_FILA--;
                Pessoa::mulher.unlock();

        }
        else {
            if((Banheiro::HOMENS_FILA > 0) && (Banheiro::HOMENS_SEG < Banheiro::LIMITE_SEG)){
                Banheiro::HOMENS_FILA--;
                Pessoa::homem.unlock();
            }else{
                Pessoa::entrada.unlock();
            }
        }
    }

    if(getSexo() == 'M'){
        Pessoa::entrada.lock();

        /*Pessoa::mensagem.lock();
        std::cout << "MULHER[" << getId() << "] SAIU DO BANHEIRO" << std::endl;
        Pessoa::mensagem.unlock();*/

        Banheiro::MULHERES_BANHEIRO--;

        //2º signal = PASSAGEM DE BASTAO
        if(Banheiro::MULHERES_BANHEIRO == 0 && Banheiro::HOMENS_FILA > 0){
                Banheiro::HOMENS_FILA--;
                Pessoa::homem.unlock();
                /*Pessoa::mensagem.lock();
                cout << "OPA2" <<endl;
                Pessoa::mensagem.unlock();*/
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

    Pessoa::TempoDeVolta();
    /*Pessoa::mensagem.lock();
    cout << "Deu uma volta" <<endl;
    Pessoa::mensagem.unlock();*/

}

void Pessoa::Run(){
    while(Banheiro::UTILIZACAO_B < Banheiro::MAX_UTILIZACAO){
            EntraBanheiro();
            if(Banheiro::UTILIZACAO_B >= Banheiro::MAX_UTILIZACAO){// QUANDO O PROGRAMA ACABA, LIBERANDO TODAS AS PESSOAS

                Pessoa::mulher.unlock();
                Pessoa::homem.unlock();

                Pessoa::entrada.unlock();
                break;
            }

            SaiBanheiro();

            darVolta();


    }
        Pessoa::mensagem.lock();
        cout << " " << getSexo() << " [ " << getId() << " ] Saindo do programa" << endl;
        Pessoa::mensagem.unlock();

}
