# Terminal de Linha de Comando - shell
## Objetivos
> Proporcionar aplicação prática dos conceitos de gerenciamento de processos em sistemas operacionais Unix e derivados.
## Introdução
> Uma shell é uma interface de linha de comando que permite aos usuários interagir com o sistema operacional. Ela interpreta e executa comandos, como navegação de diretórios, execução de programas e manipulação de arquivos. Basicamente, a shell serve como uma ponte entre o usuário e o núcleo do sistema operacional. 
## Variáveis de Ambiente
> Ambiente é uma área usada pela shell para manter o controle sobre as configurações e detalhes de compilação. O ambiente é compilado toda vez que uma nova sessão é iniciada. As variáveis de Ambiente são as que definem as informações de compilação.
> ### São exemplos de Variáveis de Ambiente:
> - **PATH**: Contém os diretórios a serem usados para execução de comandos externos;
> - **HOME**: Contém o caminho do diretório home do usuário;
> - **PWD**: Contém o diretório de trabalho atual;
> - **OLDPWD**: Contém o diretório de trabalho antigo;
## Chamadas de Sistema
> Chamadas de sistema são funções que o Sistema Operacional disponibiliza para acessar os recursos que o mesmo gerencia. Para facilitar o uso dessas funções são desenvolvidos programas de sistemas, que permitem ao usuário realizar tarefas administrativas através da execução de programas, ao invés de fazer ele mesmo um programa.
> ### Chamadas de Sistema Utilizadas
> - **getenv()**
> - **setenv()**
> - **fork()**
> - **execve()**
## Comandos Internos
> São comandos codificados internamente na shell, sem a necessidade de verificar programas externos para execução
> ### São exemplos de comandos internos:
> - **exit**: Sai da shell;
> - **pwd**: Mostra o diretório atual;
> - **cd *dir***: `cd dir` muda o diretório atual de trabalho para `dir`, caso exista. Usar `cd` ou `cd ~` leva para o diretório home, enquanto `cd -` leva para o antigo diretório de trabalho;
> - **history *[-c] [offset]***: `history` mostra os últimos 10 comandos executados na shell. `history -c` limpa o histórico. `history [offset]` executa o comando número *[offset]*.
## Comandos Externos
> São comandos que correspondem a programas executáveis armazenados em algum diretório do sistema. A shell deve localizar e executar esses programas. Em sistemas UNIX há uma variável de ambiente, chamada `PATH`, que armazena uma *string* contendo todos esses diretórios.
> ### São exemplos de comandos externos:
> - **ls** -> Lista os arquivos localizados em um determinado diretório. Caso não seja especificado o diretório, usa o diretório de trabalho atual. Localizado em `/bin/ls`;
> - **cp** -> Copia conteúdo de um arquivo para outro. Localizado em `/bin/cp`;
> - **rm** -> Remove arquivos. Localizado em `/bin/rm`;
> - **mv** -> Muda de lugar/nome um arquivo. Pode mudar um conjunto de arquivos, se o destino for um diretório Localizado em `/bin/mv`;
> - **cat** -> Mostra no terminal o conteúdo de um arquivo. Considera arquivos em modo texto, mostrando cada byte como um valor ASCCII. Localizado em `/bin/cat`.
