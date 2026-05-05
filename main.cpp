#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Astronauta {
    string cpf;
    string nome;
    int idade;
    bool vivo;
    bool disponivel;
    vector<int> voosCursados;
};

struct Voo {
    int codigo;
    vector<string> cpfsPassageiros;
    int estado; // 0 = planejado, 1 = em curso, 2 = finalizado com sucesso, 3 = finalizado com explosao
};

vector<Astronauta> astronautas;
vector<Voo> voos;

int buscarAstronauta(string cpf) {
    for (int i = 0; i < astronautas.size(); i++) {
        if (astronautas[i].cpf == cpf) return i;
    }
    return -1;
}

int buscarVoo(int codigo) {
    for (int i = 0; i < voos.size(); i++) {
        if (voos[i].codigo == codigo) return i;
    }
    return -1;
}

void cadastrarAstronauta(string cpf, int idade, string nome) {
    if (buscarAstronauta(cpf) != -1) {
        cout << "Erro: CPF " << cpf << " ja cadastrado." << endl;
        return;
    }

    Astronauta a;
    a.cpf = cpf;
    a.nome = nome;
    a.idade = idade;
    a.vivo = true;
    a.disponivel = true;

    astronautas.push_back(a);
    cout << "Astronauta " << nome << " cadastrado." << endl;
}

void cadastrarVoo(int codigo) {
    if (buscarVoo(codigo) != -1) {
        cout << "Erro: voo " << codigo << " ja existe." << endl;
        return;
    }

    Voo v;
    v.codigo = codigo;
    v.estado = 0;

    voos.push_back(v);
    cout << "Voo " << codigo << " cadastrado." << endl;
}

void adicionarAstronauta(string cpf, int codigo) {
    int ia = buscarAstronauta(cpf);
    if (ia == -1) {
        cout << "Erro: astronauta nao encontrado." << endl;
        return;
    }

    int iv = buscarVoo(codigo);
    if (iv == -1) {
        cout << "Erro: voo nao encontrado." << endl;
        return;
    }

    if (voos[iv].estado != 0) {
        cout << "Erro: voo nao esta planejado." << endl;
        return;
    }

    if (!astronautas[ia].vivo) {
        cout << "Erro: astronauta esta morto." << endl;
        return;
    }

    for (int i = 0; i < voos[iv].cpfsPassageiros.size(); i++) {
        if (voos[iv].cpfsPassageiros[i] == cpf) {
            cout << "Erro: astronauta ja esta nesse voo." << endl;
            return;
        }
    }

    voos[iv].cpfsPassageiros.push_back(cpf);
    cout << "Astronauta " << astronautas[ia].nome << " adicionado ao voo " << codigo << "." << endl;
}

void removerAstronauta(string cpf, int codigo) {
    int ia = buscarAstronauta(cpf);
    if (ia == -1) {
        cout << "Erro: astronauta nao encontrado." << endl;
        return;
    }

    int iv = buscarVoo(codigo);
    if (iv == -1) {
        cout << "Erro: voo nao encontrado." << endl;
        return;
    }

    if (voos[iv].estado != 0) {
        cout << "Erro: voo nao esta planejado." << endl;
        return;
    }

    for (int i = 0; i < voos[iv].cpfsPassageiros.size(); i++) {
        if (voos[iv].cpfsPassageiros[i] == cpf) {
            voos[iv].cpfsPassageiros.erase(voos[iv].cpfsPassageiros.begin() + i);
            cout << "Astronauta " << astronautas[ia].nome << " removido do voo " << codigo << "." << endl;
            return;
        }
    }

    cout << "Erro: astronauta nao esta nesse voo." << endl;
}

void lancarVoo(int codigo) {
    int iv = buscarVoo(codigo);
    if (iv == -1) {
        cout << "Erro: voo nao encontrado." << endl;
        return;
    }

    if (voos[iv].estado != 0) {
        cout << "Erro: voo nao esta planejado." << endl;
        return;
    }

    if (voos[iv].cpfsPassageiros.size() == 0) {
        cout << "Erro: voo sem astronautas." << endl;
        return;
    }

    for (int i = 0; i < voos[iv].cpfsPassageiros.size(); i++) {
        int ia = buscarAstronauta(voos[iv].cpfsPassageiros[i]);
        if (!astronautas[ia].vivo) {
            cout << "Erro: " << astronautas[ia].nome << " esta morto." << endl;
            return;
        }
        if (!astronautas[ia].disponivel) {
            cout << "Erro: " << astronautas[ia].nome << " esta indisponivel." << endl;
            return;
        }
    }

    voos[iv].estado = 1;

    for (int i = 0; i < voos[iv].cpfsPassageiros.size(); i++) {
        int ia = buscarAstronauta(voos[iv].cpfsPassageiros[i]);
        astronautas[ia].disponivel = false;
        astronautas[ia].voosCursados.push_back(codigo);
    }

    cout << "Voo " << codigo << " lancado." << endl;
}

void explodirVoo(int codigo) {
    int iv = buscarVoo(codigo);
    if (iv == -1) {
        cout << "Erro: voo nao encontrado." << endl;
        return;
    }

    if (voos[iv].estado != 1) {
        cout << "Erro: voo nao esta em curso." << endl;
        return;
    }

    voos[iv].estado = 3;

    for (int i = 0; i < voos[iv].cpfsPassageiros.size(); i++) {
        int ia = buscarAstronauta(voos[iv].cpfsPassageiros[i]);
        astronautas[ia].vivo = false;
        astronautas[ia].disponivel = false;
    }

    cout << "Voo " << codigo << " explodiu." << endl;
}

void finalizarVoo(int codigo) {
    int iv = buscarVoo(codigo);
    if (iv == -1) {
        cout << "Erro: voo nao encontrado." << endl;
        return;
    }

    if (voos[iv].estado != 1) {
        cout << "Erro: voo nao esta em curso." << endl;
        return;
    }

    voos[iv].estado = 2;

    for (int i = 0; i < voos[iv].cpfsPassageiros.size(); i++) {
        int ia = buscarAstronauta(voos[iv].cpfsPassageiros[i]);
        astronautas[ia].disponivel = true;
    }

    cout << "Voo " << codigo << " finalizado com sucesso." << endl;
}

void listarVoos() {
    string nomeEstado[] = {"planejado", "em curso", "finalizado com sucesso", "finalizado com explosao"};

    cout << "Voos" << endl;
    for (int estado = 0; estado <= 3; estado++) {
        cout << nomeEstado[estado] << ":" << endl;
        bool achou = false;
        for (int i = 0; i < voos.size(); i++) {
            if (voos[i].estado == estado) {
                achou = true;
                cout << "  codigo: " << voos[i].codigo << endl;
                for (int j = 0; j < voos[i].cpfsPassageiros.size(); j++) {
                    int ia = buscarAstronauta(voos[i].cpfsPassageiros[j]);
                    cout << "    cpf: " << voos[i].cpfsPassageiros[j] << " nome: " << astronautas[ia].nome << endl;
                }
                if (voos[i].cpfsPassageiros.size() == 0) {
                    cout << "    sem passageiros" << endl;
                }
            }
        }
        if (!achou) cout << "  nenhum" << endl;
    }
}

void listarMortos() {
    cout << "Mortos" << endl;
    bool achou = false;
    for (int i = 0; i < astronautas.size(); i++) {
        if (!astronautas[i].vivo) {
            achou = true;
            cout << "cpf: " << astronautas[i].cpf << " nome: " << astronautas[i].nome << endl;
            cout << "  voos: ";
            if (astronautas[i].voosCursados.size() == 0) {
                cout << "nenhum";
            } else {
                for (int j = 0; j < astronautas[i].voosCursados.size(); j++) {
                    cout << astronautas[i].voosCursados[j];
                    if (j < astronautas[i].voosCursados.size() - 1) cout << ", ";
                }
            }
            cout << endl;
        }
    }
    if (!achou) cout << "nenhum" << endl;
}

int main() {
    string comando;
    while (cin >> comando) {
        if (comando == "FIM") {
            break;
        }
        if (comando == "CADASTRAR_ASTRONAUTA") {
            string cpf, nome;
            int idade;
            cin >> cpf >> idade;
            getline(cin >> ws, nome);
            cadastrarAstronauta(cpf, idade, nome);
        } else if (comando == "CADASTRAR_VOO") {
            int codigo;
            cin >> codigo;
            cadastrarVoo(codigo);
        } else if (comando == "ADICIONAR_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            adicionarAstronauta(cpf, codigo);
        } else if (comando == "REMOVER_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            removerAstronauta(cpf, codigo);
        } else if (comando == "LANCAR_VOO") {
            int codigo;
            cin >> codigo;
            lancarVoo(codigo);
        } else if (comando == "EXPLODIR_VOO") {
            int codigo;
            cin >> codigo;
            explodirVoo(codigo);
        } else if (comando == "FINALIZAR_VOO") {
            int codigo;
            cin >> codigo;
            finalizarVoo(codigo);
        } else if (comando == "LISTAR_VOOS") {
            listarVoos();
        } else if (comando == "LISTAR_MORTOS") {
            listarMortos();
        }
    }

    return 0;
}