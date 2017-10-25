#ifndef BANHEIRO_H_INCLUDED
#define BANHEIRO_H_INCLUDED
#include <mutex>

class Banheiro{
public:
    static std::mutex entrada;
    static int CAPACIDADE;
    static int HOMENS_BANHEIRO;
    static int MULHERES_BANHEIRO;
    static int HOMENS_FILA;
    static int MULHERES_FILA;
    static int HOMENS_SEG;
    static int MULHERES_SEG;
    static int MAX_UTILIZACAO;
    static int UTILIZACAO_B;
    static int LIMITE_SEG;

};


#endif // BANHEIRO_H_INCLUDED
