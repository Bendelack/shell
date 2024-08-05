#include <iostream> // lib de entrada e saída
#include <unistd.h> // lib padrão para acessar funções de sistema do unix
#include <sys/wait.h> // manipulação de processos
#include <vector> // lib vector
#include <string> // lib de string
#include <sstream> // manipulação de strings

std::vector<std::string> history; // vector para armazenar o histórico

void process_command(std::string command) {
    history.push_back(command); // adiciona o comando no histórico
    // comandos internos...
    // fecha a shell se o comando for 'exit'
    if ( command == "exit" ) exit(0); // exit
    else if ( command == "pwd" ){ // mostra o diretório atual
	    std::cout << std::getenv("PWD") << std::endl; // pega o diretório atual da variável de ambiente PWD usando a chamada de sistema getenv()
	    return; // retorna 'nada' e segue a execução normal da shell
    }
    // dividindo o comando para pegar possíveis argumentos
    std::string arguments[16]; // vetor para salvar comando e argumentos
    int x = 0; // armazena a quantidade de elementos do vetor de argumentos

    // classe stream para dividir a string
    std::istringstream commandStream(command);

    std::string element; // armazena cada elemento dividido

    char separator; // elemento o elemento separador da string
    separator = ' '; // separador de argumentos

    // separando string por ' '
    while(std::getline(commandStream, element, separator))
        arguments[x++] = element;

    // mais comandos internos
    if (arguments[0] == "cd"){ //cd dir
        const char * changeToDir; // armazena o diretório para o qual será mudado
        if(x==1) // se ouver apenas um argumento, ou seja, 'cd'
            changeToDir = getenv("HOME"); // mudar para home
        else if(x==2 && arguments[1]=="~") //se ouver dois argumentos e o segundo for '~'
            changeToDir = getenv("HOME"); // mudar para home
        else if(x==2 && arguments[1]=="-") // se ouver dois argumentos e o segundo for '-'
            changeToDir = getenv("OLDPWD"); // mudar para o diretório que estava antes
        else // se não for nenhuma das anteriores
            changeToDir = arguments[1].c_str(); // pega o valor passado como argumento
        if(chdir(changeToDir) == -1){ // muda o diretório
            perror("error to change directory"); // se houver falhas, mostra a mensagem
        } else {
            setenv("OLDPWD", getenv("PWD"), 1); // caminho antigo recebe o caminho atual. 
            setenv("PWD", changeToDir, 1); // caminho atual recebe o novo diretório
            std::cout << "change to dir: " << changeToDir << std::endl; // mostra para qual diretório mudou
        }
        return; // retorna 'nada' e segue o fluxo normal de execução da shell
    } else if (arguments[0] == "history"){// history
        size_t start = 0; // começa no zero quando tiver 10 elementos ou menos
        int offset = history.size() - 1; // indice do comando começa em (tamanho do vetor menos 1) quando tiver 10 ou menos elementos
        if(history.size() > 10){ // se o tamanho for maior que 10
            start = history.size() - 10; // começa em tamanho menos 10
            offset = 9; // indice começa em 9
        }
        if(x==1) // se tiver apenas um argumento, ou seja, 'history'
            for(size_t i = start; i < history.size(); i++) // mostra os últimos 10 comandos na tela
                std::cout << offset-- << " "  << history[i] << std::endl;
        else if (x == 2 && arguments[1] == "-c") // se houver dois arumentos e o segundo for '-c'
            history.clear(); // limpa o histórico
        else { // se não for nenhuma das anteriores espera-se que seja 'history offset', sendo offset um valor inteiro correspondente ao indice de um comando do histórico
            offset = std::stoi(arguments[1]); // converter o segundo argumento para inteiro
            process_command(history[(history.size())-offset-2]); // precessa o comando chamando própria função novamente
        }
        return; // retorna 'nada' e segue o fluxo normal de execução da shell
    }
    //comandos externos
    // vetor para armezenar os diretórios externos para buscar arquivos executáveis
    std::string argsPath[32];
    int y = 0; // armazena a quantidade de diretórios disponíveis

    std::string envPATH = std::getenv("PATH"); // ler a variável de ambiente PATH usando a chamada de sistema PATH

    // classe stream para dividir a string
    std::istringstream pathStream(envPATH); // PATH armazena os diretórios separando-os por ':'

    separator = ':'; // separador de diretórios

    // separando string por ':'
    while(std::getline(pathStream, element, separator))
        argsPath[y++] = element;

    /*  Se for caminho relativo, procurar o comando na lista de diretórios
        Se for absoluto verifica se comando existe
    */
   // algumas variáveis booleanas para ajudar na execução do programa
    bool finded = false; // armazenará 'true' caso o comando seja encontrado na lista de diretórios
    bool executed = false; // armazenará 'true' caso o comando seja executado
    for (int i = 0; i < y; i++){ // para cada diretório na lista de diretórios de PATH
	    std::string absolute_path = argsPath[i] + "/" + arguments[0]; // criando caminho absoluto para o comando
        if (access(absolute_path.c_str(), F_OK) == 0) { // Arquivo existe no diretório
	        finded = true; // arquivo encontrado
            if (access(absolute_path.c_str(), X_OK) == 0) { // Arquivo é executável
		        executed = true; // arquivo executável
                pid_t pid = fork(); // cria um processo filho para executar o programa
                if (pid < 0){ // Erro
                    std::cout << "Erro de execução!" << std::endl; // mostra mensagem de erro
                    return; // retorna 'nada' e segue o fluxo normal de execução da shell
                } else if (pid == 0){ //processo filho
                    char * argv[x+1]; // cria um vetor com o tamanho de argumentos informados + 1 (último será nulo para indicar o fim dos argumentos)
                    argv[0] = (char *)arguments[0].c_str(); // o primeiro é o próprio comando
                    argv[x] = nullptr; // o último é nulo
                    for (int z = 1; z < x; z++) // para cada argumeto informado, além do próprio comando
                        argv[z] = (char *)arguments[z].c_str(); // armazena no vetor de argumentos
                    execve(absolute_path.c_str(), argv, NULL); // executa o comando
                } else { // Processo pai
                    /* Deve adicionar processo filho na lista (std::vector) 
                       de processos em execução para gerenciar background. */
                    /* Processo pai espera processo filho terminar. */
                    waitpid(pid, nullptr, 0);
                }
		        return;  // retorna 'nada' e segue o fluxo normal de execução da shell
	        }
        }
    }
    // caso toda a lista de diretórios tenha sido toda percorrida, vamos a algumas considerações
    // 1º - arquivo encontrado mas não executável
    if(finded == true && executed == false) // Arquivo não é executável
        std::cout << "permission denied: " << command << std::endl;
    // 2º - arquivo não foi encontrado
    if(finded == false) // Arquivo não existe
        std::cout << "Command not found: " << command << std::endl;
}

// função principal para execução da shell
int main() {
    while (true) { // roda em laço infinito
        std::cout << "$ "; // cifrão padrão da bash
        std::string command; // armazena o comando 
        getline(std::cin, command); // pega a linha digitada 
        process_command(command); // processa o comando
    }
    return 0;
}
