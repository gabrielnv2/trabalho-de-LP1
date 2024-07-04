#include <iostream>
#include <vector>
#include <string>

class Astronauta {
public:
    std::string cpf;
    std::string nome;
    int idade;
    bool disponivel;
    bool morto;
    std::vector<int> voos;

    Astronauta(std::string cpf, std::string nome, int idade)
        : cpf(cpf), nome(nome), idade(idade), disponivel(true), morto(false) {}

    void addVoo(int vooCodigo) {
        voos.push_back(vooCodigo);
    }

    std::string getInfo() const {
        return "CPF: " + cpf + ", Nome: " + nome + ", Idade: " + std::to_string(idade);
    }
};

class Voo {
public:
    int codigo;
    std::vector<std::string> passageiros;
    enum Status { PLANEJADO, EM_CURSO, FINALIZADO } status;
    bool sucesso;

    Voo(int codigo) : codigo(codigo), status(PLANEJADO), sucesso(false) {}

    void addPassageiro(std::string cpf) {
        passageiros.push_back(cpf);
    }

    void removePassageiro(std::string cpf) {
        for (auto it = passageiros.begin(); it != passageiros.end(); ++it) {
            if (*it == cpf) {
                passageiros.erase(it);
                return;
            }
        }
    }

    void lancarVoo() {
        if (status == PLANEJADO && passageiros.size() > 0) {
            status = EM_CURSO;
        } else {
            std::cerr << "Erro: Voo não pode ser lançado." << std::endl;
        }
    }

    void registrarExplosao() {
        if (status == EM_CURSO) {
            status = FINALIZADO;
            sucesso = false;
        } else {
            std::cerr << "Erro: Voo não está em curso." << std::endl;
        }
    }

    void finalizarVoo(bool sucesso) {
        if (status == EM_CURSO) {
            status = FINALIZADO;
            this->sucesso = sucesso;
        } else {
            std::cerr << "Erro: Voo não está em curso." << std::endl;
        }
    }

    std::string getInfo() const {
        std::string info = "Código do Voo: " + std::to_string(codigo) + ", Status: " + (status == PLANEJADO ? "Planejado" : (status == EM_CURSO ? "Em Curso" : "Finalizado")) + "\n";
        info += "Passageiros:\n";
        for (const auto& cpf : passageiros) {
            info += " - " + cpf + "\n";
        }
        return info;
    }
};

class AgenciaEspacial {
public:
    std::vector<Astronauta> astronautas;
    std::vector<Voo> voos;

    void cadastrarAstronauta(Astronauta astronauta) {
        astronautas.push_back(astronauta);
    }

    void cadastrarVoo(Voo voo) {
        voos.push_back(voo);
    }

    void adicionarAstronautaEmVoo(std::string cpf, int codigoVoo) {
        auto& voo = encontrarVoo(codigoVoo);
        if (voo.status != Voo::PLANEJADO) {
            std::cerr << "Erro: Voo não está em planejamento." << std::endl;
            return;
        }

        auto& astronauta = encontrarAstronauta(cpf);
        if (!astronauta.disponivel) {
            std::cerr << "Erro: Astronauta não está disponível." << std::endl;
            return;
        }

        voo.addPassageiro(cpf);
        astronauta.addVoo(codigoVoo);
    }

    void removerAstronautaDeVoo(std::string cpf, int codigoVoo) {
        auto& voo = encontrarVoo(codigoVoo);
        if (voo.status != Voo::PLANEJADO) {
            std::cerr << "Erro: Voo não está em planejamento." << std::endl;
            return;
        }

        voo.removePassageiro(cpf);
    }

    void lancarVoo(int codigoVoo) {
        auto& voo = encontrarVoo(codigoVoo);
        voo.lancarVoo();
        for (const auto& cpf : voo.passageiros) {
            auto& astronauta = encontrarAstronauta(cpf);
            astronauta.disponivel = false;
        }
    }

    void registrarExplosao(int codigoVoo) {
        auto& voo = encontrarVoo(codigoVoo);
        voo.registrarExplosao();
        for (const auto& cpf : voo.passageiros) {
            auto& astronauta = encontrarAstronauta(cpf);
            astronauta.morto = true;
            astronauta.disponivel = false;
        }
    }

    void finalizarVoo(int codigoVoo, bool sucesso) {
        auto& voo = encontrarVoo(codigoVoo);
        voo.finalizarVoo(sucesso);
        if (sucesso) {
            for (const auto& cpf : voo.passageiros) {
                auto& astronauta = encontrarAstronauta(cpf);
                astronauta.disponivel = true;
            }
        }
    }

    void listarVoos() const {
        for (const auto& voo : voos) {
            std::cout << voo.getInfo() << std::endl;
        }
    }

    void listarAstronautas() const {
        for (const auto& astronauta : astronautas) {
            std::cout << astronauta.getInfo() << std::endl;
        }
    }

    void listarAstronautasMortos() const {
        for (const auto& astronauta : astronautas) {
            if (astronauta.morto) {
                std::cout << astronauta.getInfo() << std::endl;
                std::cout << "Voos: ";
                for (const auto& voo : astronauta.voos) {
                    std::cout << voo << " ";
                }
                std::cout << std::endl;
            }
        }
    }

private:
    Voo& encontrarVoo(int codigoVoo) {
        for (auto& voo : voos) {
            if (voo.codigo == codigoVoo) {
                return voo;
            }
        }
        throw std::runtime_error("Voo não encontrado.");
    }

    Astronauta& encontrarAstronauta(std::string cpf) {
        for (auto& astronauta : astronautas) {
            if (astronauta.cpf == cpf) {
                return astronauta;
            }
        }
        throw std::runtime_error("Astronauta não encontrado.");
    }
};

void menu(AgenciaEspacial& agencia) {
    int opcao;
    do {
        std::cout << "Menu Principal:" << std::endl;
        std::cout << "1. Cadastrar Astronauta" << std::endl;
        std::cout << "2. Cadastrar Voo" << std::endl;
        std::cout << "3. Adicionar Astronauta em Voo" << std::endl;
        std::cout << "4. Remover Astronauta de Voo" << std::endl;
        std::cout << "5. Lançar Voo" << std::endl;
        std::cout << "6. Registrar Explosão de Voo" << std::endl;
        std::cout << "7. Registrar Voo Finalizado com Sucesso" << std::endl;
        std::cout << "8. Listar Astronautas" << std::endl;
        std::cout << "9. Listar Voos" << std::endl;
        std::cout << "10. Listar Astronautas Mortos" << std::endl;
        std::cout << "11. Sair" << std::endl;
        std::cout << "Digite sua opção: ";
        std::cin >> opcao;

        switch (opcao) {
            case 1: {
                std::string cpf, nome;
                int idade;
                std::cout << "Digite o CPF do astronauta: ";
                std::cin >> cpf;
                std::cout << "Digite o nome do astronauta: ";
                std::cin >> nome;
                std::cout << "Digite a idade do astronauta: ";
                std::cin >> idade;
                agencia.cadastrarAstronauta(Astronauta(cpf, nome, idade));
                break;
            }
            case 2: {
                int codigo;
                std::cout << "Digite o código do voo: ";
                std::cin >> codigo;
                agencia.cadastrarVoo(Voo(codigo));
                break;
            }
            case 3: {
                std::string cpf;
                int codigoVoo;
                std::cout << "Digite o CPF do astronauta: ";
                std::cin >> cpf;
                std::cout << "Digite o código do voo: ";
                std::cin >> codigoVoo;
                agencia.adicionarAstronautaEmVoo(cpf, codigoVoo);
                break;
            }
            case 4: {
                std::string cpf;
                int codigoVoo;
                std::cout << "Digite o CPF do astronauta: ";
                std::cin >> cpf;
                std::cout << "Digite o código do voo: ";
                std::cin >> codigoVoo;
                agencia.removerAstronautaDeVoo(cpf, codigoVoo);
                break;
            }
            case 5: {
                int codigoVoo;
                std::cout << "Digite o código do voo: ";
                std::cin >> codigoVoo;
                agencia.lancarVoo(codigoVoo);
                break;
            }
            case 6: {
                int codigoVoo;
                std::cout << "Digite o código do voo: ";
                std::cin >> codigoVoo;
                agencia.registrarExplosao(codigoVoo);
                break;
            }
            case 7: {
                int codigoVoo;
                bool sucesso;
                std::cout << "Digite o código do voo: ";
                std::cin >> codigoVoo;
                std::cout << "O voo foi finalizado com sucesso? (1 para sim, 0 para não): ";
                std::cin >> sucesso;
                agencia.finalizarVoo(codigoVoo, sucesso);
                break;
            }
            case 8: {
                agencia.listarAstronautas();
                break;
            }
            case 9: {
                agencia.listarVoos();
                break;
            }
            case 10: {
                agencia.listarAstronautasMortos();
                break;
            }
            case 11:
                std::cout << "Saindo do sistema..." << std::endl;
                break;
            default:
                std::cout << "Opção inválida. Tente novamente." << std::endl;
                break;
        }
    } while (opcao != 11);
}

int main() {
    AgenciaEspacial agencia;
    menu(agencia);
    return 0;
}
