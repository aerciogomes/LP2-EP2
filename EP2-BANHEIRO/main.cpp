#include "Pessoa.h"
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include "Pessoa.h"

#define MAX 20

using namespace std;

int main() {

    Pessoa *pessoa[MAX];

    for (int i = 0; i < MAX; i++){
        if(i%2)
            pessoa[i] = new Pessoa(i, 'H');
        else
            pessoa[i] = new Pessoa(i, 'M');
    }

    Pessoa::mulher.lock();
    Pessoa::homem.lock();

    vector<thread> tPessoas;

    for (auto &p : pessoa) tPessoas.push_back(thread(&Pessoa::Run, p));

    for (auto &th : tPessoas) th.join();

    for (int i = 0; i < MAX; i++)
    	delete pessoa[i];

	return 0;
}

