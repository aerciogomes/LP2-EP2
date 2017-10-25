#ifndef PESSOA_H_INCLUDED
#define PESSOA_H_INCLUDED
#include <mutex>

class Pessoa{
    int id;
    char sexo;
public:
    Pessoa(int, char);
    ~Pessoa(){};

    int getId();
    char getSexo();
    static std::mutex entrada;
    static std::mutex homem;
    static std::mutex mulher;
    static std::mutex mensagem;
    static std::mutex slept;

    void EsperaNoBanheiro();
    void TempoDeVolta();
    void Tempo(int);

    void EntraBanheiro();
    void SaiBanheiro();
    void darVolta();
    void Run();

};


#endif // PESSOA_H_INCLUDED
