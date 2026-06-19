Exercício 1(a) - Observação de múltiplas execuções:
    Ao executar o programa repetidas vezes no terminal (ex: mpiexec -n 8 ./hello_os), duas observações confirmam a arquitetura SPMD do MPI:
    1. Todos os PIDs são diferentes: O MPI não usa simples threads, mas sim processos reais e isolados no Sistema Operacional. Cada rank possui seu próprio espaço de memória e PID exclusivo.
    2. A coluna de CPU muda entre execuções: O ambiente MPI delega ao escalonador (scheduler) do kernel do Linux a decisão de qual núcleo físico processará cada tarefa. Como o escalonamento é dinâmico e depende da carga atual da máquina, os processos "saltam" de núcleo a cada execução.

    ```vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 4 ./hello_os
Hello from rank 0/4 -- PID = 29237, CPU = 3
Hello from rank 1/4 -- PID = 29238, CPU = 7
Hello from rank 2/4 -- PID = 29239, CPU = 1
Hello from rank 3/4 -- PID = 29242, CPU = 5
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 4 ./hello_os
Hello from rank 0/4 -- PID = 29255, CPU = 0
Hello from rank 1/4 -- PID = 29256, CPU = 2
Hello from rank 2/4 -- PID = 29257, CPU = 1
Hello from rank 3/4 -- PID = 29259, CPU = 3
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 4 ./hello_os
Hello from rank 0/4 -- PID = 29285, CPU = 0
Hello from rank 1/4 -- PID = 29286, CPU = 2
Hello from rank 2/4 -- PID = 29287, CPU = 3
Hello from rank 3/4 -- PID = 29289, CPU = 4
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 4 ./hello_os
Hello from rank 0/4 -- PID = 29327, CPU = 1
Hello from rank 1/4 -- PID = 29328, CPU = 6
Hello from rank 2/4 -- PID = 29329, CPU = 0
Hello from rank 3/4 -- PID = 29331, CPU = 2```

vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 8 --oversubscribe ./hello_os
Hello from rank 0/8 -- PID = 33737, CPU = 4
Hello from rank 1/8 -- PID = 33738, CPU = 3
Hello from rank 2/8 -- PID = 33739, CPU = 6
Hello from rank 3/8 -- PID = 33740, CPU = 2
Hello from rank 4/8 -- PID = 33741, CPU = 1
Hello from rank 5/8 -- PID = 33745, CPU = 5
Hello from rank 6/8 -- PID = 33747, CPU = 3
Hello from rank 7/8 -- PID = 33749, CPU = 0
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 8 --oversubscribe ./hello_os
Hello from rank 0/8 -- PID = 33821, CPU = 4
Hello from rank 1/8 -- PID = 33822, CPU = 7
Hello from rank 2/8 -- PID = 33823, CPU = 0
Hello from rank 3/8 -- PID = 33824, CPU = 6
Hello from rank 4/8 -- PID = 33826, CPU = 3
Hello from rank 5/8 -- PID = 33829, CPU = 1
Hello from rank 6/8 -- PID = 33832, CPU = 5
Hello from rank 7/8 -- PID = 33835, CPU = 2
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 8 --oversubscribe ./hello_os
Hello from rank 0/8 -- PID = 33869, CPU = 4
Hello from rank 1/8 -- PID = 33870, CPU = 6
Hello from rank 2/8 -- PID = 33871, CPU = 1
Hello from rank 3/8 -- PID = 33872, CPU = 7
Hello from rank 4/8 -- PID = 33875, CPU = 0
Hello from rank 5/8 -- PID = 33877, CPU = 2
Hello from rank 6/8 -- PID = 33880, CPU = 3
Hello from rank 7/8 -- PID = 33882, CPU = 5
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 4 ./hello_os
Hello from rank 0/4 -- PID = 33936, CPU = 4
Hello from rank 1/4 -- PID = 33937, CPU = 2
Hello from rank 2/4 -- PID = 33938, CPU = 0
Hello from rank 3/4 -- PID = 33939, CPU = 6
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 4 ./hello_os
Hello from rank 0/4 -- PID = 33963, CPU = 0
Hello from rank 1/4 -- PID = 33964, CPU = 2
Hello from rank 2/4 -- PID = 33965, CPU = 7
Hello from rank 3/4 -- PID = 33967, CPU = 4
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 4 ./hello_os
Hello from rank 0/4 -- PID = 33984, CPU = 2
Hello from rank 1/4 -- PID = 33985, CPU = 0
Hello from rank 2/4 -- PID = 33986, CPU = 4
Hello from rank 3/4 -- PID = 33987, CPU = 6


Exercício 1(b) - Justificativa contra Deadlock:
    Em uma comunicação em anel, uma abordagem ingênua seria todos os processos chamarem MPI_Recv seguido de MPI_Send. O problema dessa estratégia é que o MPI_Recv é uma função bloqueante. Se todos os processos (do rank 0 ao p-1) ficarem aguardando o recebimento de uma mensagem, nenhum processo chegará à linha do código que executa o MPI_Send. Isso gera um ciclo de espera infinita, conhecido como deadlock.

    A solução escolhida foi utilizar o MPI_Sendrecv. Esta função encapsula o envio e o recebimento em uma única chamada de sistema, instruindo a biblioteca MPI a gerenciar os buffers internos de forma segura e paralela, quebrando o ciclo de espera e permitindo que as mensagens transitem sem o risco de travamento do programa. O uso de (k - 1 + p) % p garante a integridade aritmética para o cálculo da origem, já que o operador de módulo % em C pode resultar em valores negativos.

    ```vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 4 ./hello_ring
Rank 0 coletou: Greeting from rank 3 of 4! (Veio do Rank 3)
Rank 0 coletou: Greeting from rank 0 of 4! (Veio do Rank 0)
Rank 0 coletou: Greeting from rank 1 of 4! (Veio do Rank 1)
Rank 0 coletou: Greeting from rank 2 of 4! (Veio do Rank 2)
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 4 ./hello_ring
Rank 0 coletou: Greeting from rank 3 of 4! (Veio do Rank 3)
Rank 0 coletou: Greeting from rank 0 of 4! (Veio do Rank 0)
Rank 0 coletou: Greeting from rank 1 of 4! (Veio do Rank 1)
Rank 0 coletou: Greeting from rank 2 of 4! (Veio do Rank 2)
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 4 ./hello_ring
Rank 0 coletou: Greeting from rank 3 of 4! (Veio do Rank 3)
Rank 0 coletou: Greeting from rank 0 of 4! (Veio do Rank 0)
Rank 0 coletou: Greeting from rank 1 of 4! (Veio do Rank 1)
Rank 0 coletou: Greeting from rank 2 of 4! (Veio do Rank 2)```


vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 4 ./hello_ring
Rank 0 recebeu a mensagem: Greeting from rank 3 of 4! (do Rank 3)
Rank 0 coletou do Rank 1 a mensagem: Greeting from rank 0 of 4!
Rank 0 coletou do Rank 2 a mensagem: Greeting from rank 1 of 4!
Rank 0 coletou do Rank 3 a mensagem: Greeting from rank 2 of 4!
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex1_hello_os$ mpiexec -n 8 --oversubscribe ./hello_ring
Rank 0 recebeu a mensagem: Greeting from rank 7 of 8! (do Rank 7)
Rank 0 coletou do Rank 1 a mensagem: Greeting from rank 0 of 8!
Rank 0 coletou do Rank 2 a mensagem: Greeting from rank 1 of 8!
Rank 0 coletou do Rank 3 a mensagem: Greeting from rank 2 of 8!
Rank 0 coletou do Rank 4 a mensagem: Greeting from rank 3 of 8!
Rank 0 coletou do Rank 5 a mensagem: Greeting from rank 4 of 8!
Rank 0 coletou do Rank 6 a mensagem: Greeting from rank 5 of 8!
Rank 0 coletou do Rank 7 a mensagem: Greeting from rank 6 of 8!


Resultados do Exercício 1(a)
As execuções sucessivas de mpiexec -n 4 ./hello_os demonstraram na prática o modelo SPMD do MPI. Em cada rodada, os PIDs gerados foram únicos e variaram, comprovando que o MPI cria processos autônomos no SO, e não apenas threads leves. Além disso, a alocação de CPUs (ex: rank 0 variando entre as CPUs 0, 1 e 3) evidenciou a atuação do escalonador dinâmico do kernel do Linux distribuindo a carga entre os núcleos disponíveis.  

Resultados do Exercício 1(b)
A topologia em anel foi implementada utilizando a função MPI_Sendrecv. A saída impressa pelo rank 0 validou o tráfego correto das mensagens: o rank 0 recebeu do 3, o 1 recebeu do 0, o 2 recebeu do 1 e o 3 recebeu do 2. A escolha do MPI_Sendrecv foi essencial para encapsular o envio e recebimento de forma assíncrona nos buffers internos, quebrando o ciclo de espera circular que inevitavelmente causaria deadlock se usássemos chamadas bloqueantes simples de MPI_Recv seguidas de MPI_Send.

A abordagem escolhida para evitar o deadlock no anel foi a utilização da função MPI_Sendrecv. Como o MPI_Recv é estritamente bloqueante, uma abordagem ingênua geraria uma espera cíclica. Optou-se por MPI_Sendrecv em vez de reordenar manualmente as chamadas de envio e recebimento via código, pois essa função delega à própria biblioteca MPI a orquestração simultânea e segura da comunicação, resultando em um código mais limpo, portável e blindado contra deadlocks.

Exercício 2 


a. vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex2_trap$ echo "0 3.141592653589793 10" | mpiexec -n 3 ./mpi_trap_generalized
Enter a, b, n: Rank 0 executou 4 trapezios no intervalo [0.000000, 1.256637]
Rank 1 executou 3 trapezios no intervalo [1.256637, 2.199115]
Rank 2 executou 3 trapezios no intervalo [2.199115, 3.141593]
With n = 10 trapezoids, integral of f on [0, 3.14159] = 1.983523537509454e+00

Exercício 2(b) - Estudo de Escalabilidade Forte (n=107):

vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex2_trap$ echo "0 3.141592653589793 10000000" | mpiexec -n 1 ./mpi_trap_generalized
Enter a, b, n: Rank 0 executou 10000000 trapezios no intervalo [0.000000, 3.141593]
With n = 10000000 trapezoids, integral of f on [0, 3.14159] = 1.999999999999809e+00
Tempo de execucao = 0.104397 segundos
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex2_trap$ echo "0 3.141592653589793 10000000" | mpiexec -n 2 ./mpi_trap_generalized
Enter a, b, n: Rank 0 executou 5000000 trapezios no intervalo [0.000000, 1.570796]
Rank 1 executou 5000000 trapezios no intervalo [1.570796, 3.141593]
With n = 10000000 trapezoids, integral of f on [0, 3.14159] = 1.999999999999962e+00
Tempo de execucao = 0.060864 segundos
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex2_trap$ echo "0 3.141592653589793 10000000" | mpiexec -n 4 ./mpi_trap_generalized
Enter a, b, n: Rank 0 executou 2500000 trapezios no intervalo [0.000000, 0.785398]
Rank 2 executou 2500000 trapezios no intervalo [1.570796, 2.356194]
Rank 1 executou 2500000 trapezios no intervalo [0.785398, 1.570796]
Rank 3 executou 2500000 trapezios no intervalo [2.356194, 3.141593]
With n = 10000000 trapezoids, integral of f on [0, 3.14159] = 2.000000000000020e+00
Tempo de execucao = 0.056489 segundos

vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex2_trap$ echo "0 3.141592653589793 10000000" | mpiexec -n 8 --oversubscribe ./mpi_trap_generalized
Enter a, b, n: Rank 0 executou 1250000 trapezios no intervalo [0.000000, 0.392699]
Rank 1 executou 1250000 trapezios no intervalo [0.392699, 0.785398]
Rank 5 executou 1250000 trapezios no intervalo [1.963495, 2.356194]
Rank 3 executou 1250000 trapezios no intervalo [1.178097, 1.570796]
Rank 2 executou 1250000 trapezios no intervalo [0.785398, 1.178097]
Rank 7 executou 1250000 trapezios no intervalo [2.748894, 3.141593]
Rank 4 executou 1250000 trapezios no intervalo [1.570796, 1.963495]
With n = 10000000 trapezoids, integral of f on [0, 3.14159] = 1.999999999999960e+00
Tempo de execucao = 0.043774 segundos
Rank 6 executou 1250000 trapezios no intervalo [2.356194, 2.748894]



    Utilizando Ts​=0.104397 s (tempo do próprio código executado com p=1), obtivemos a seguinte tabela de desempenho:
    Número de Processos (p)	Tempo de Relógio Tp​ (s)	Speedup (Sp​=Ts​/Tp​)	Eficiência (Ep​=Sp​/p)
    1	0.104397	1.000	1.000
    2	0.060864	1.715	0.857
    4	0.056489	1.848	0.462
    8	0.043774	2.385	0.298

    Discussão: A partir de qual p a eficiência cai abaixo de 0.8 e por quê?
    Observando a tabela, a eficiência Ep​ cai drasticamente abaixo de 0.8 a partir de p=4, atingindo 0.462. Essa queda acentuada ocorre por uma combinação de fatores:

        Gargalo de Comunicação Linear: A coleta dos resultados é feita por um laço sequencial de chamadas MPI_Recv na raiz (uma comunicação em estrela, abordada na Aula 5). O custo dessa etapa cresce em O(p). Ao usarmos p=4 ou p=8, o tempo de cálculo da integral fica minúsculo, mas o overhead de ficar trocando e aguardando mensagens domina o tempo de execução.

        Oversubscription (Sobrecarga de CPU): Para executar com p=8, foi necessário utilizar a flag --oversubscribe. Isso indica que a máquina física possui menos núcleos reais do que os processos requisitados. Com o escalonador do SO sendo forçado a dividir a atenção de uma mesma CPU física para múltiplos processos (troca de contexto), o ganho paralelo despenca, justificando uma eficiência de apenas 29,8% com 8 processos.

        Lei de Amdahl: A parte puramente sequencial do código (como inicialização do ambiente MPI e leitura/escrita na tela) não é acelerada pelo paralelismo e impõe um limite máximo ao Speedup.



exercicio 2.c 

vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex2_trap$ echo "0 3.141592653589793 64" | mpiexec -n 4 ./mpi_trap_generalized
Enter a, b, n: Rank 1 executou 16 trapezios no intervalo [0.785398, 1.570796]
Rank 2 executou 16 trapezios no intervalo [1.570796, 2.356194]
Rank 3 executou 16 trapezios no intervalo [2.356194, 3.141593]
Rank 0 executou 16 trapezios no intervalo [0.000000, 0.785398]
With n = 64 trapezoids, integral of f on [0, 3.14159] = 1.999598388640037e+00
Tempo de execucao = 0.000047 segundos
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex2_trap$ echo "0 3.141592653589793 256" | mpiexec -n 4 ./mpi_trap_generalized
Enter a, b, n: Rank 1 executou 64 trapezios no intervalo [0.785398, 1.570796]
Rank 2 executou 64 trapezios no intervalo [1.570796, 2.356194]
Rank 3 executou 64 trapezios no intervalo [2.356194, 3.141593]
Rank 0 executou 64 trapezios no intervalo [0.000000, 0.785398]
With n = 256 trapezoids, integral of f on [0, 3.14159] = 1.999974900235053e+00
Tempo de execucao = 0.000043 segundos
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex2_trap$ echo "0 3.141592653589793 1024" | mpiexec -n 4 ./mpi_trap_generalized
Enter a, b, n: Rank 2 executou 256 trapezios no intervalo [1.570796, 2.356194]
Rank 3 executou 256 trapezios no intervalo [2.356194, 3.141593]
Rank 0 executou 256 trapezios no intervalo [0.000000, 0.785398]
Rank 1 executou 256 trapezios no intervalo [0.785398, 1.570796]
With n = 1024 trapezoids, integral of f on [0, 3.14159] = 1.999998431268383e+00
Tempo de execucao = 0.000043 segundos
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex2_trap$ echo "0 3.141592653589793 4096" | mpiexec -n 4 ./mpi_trap_generalized
Enter a, b, n: Rank 1 executou 1024 trapezios no intervalo [0.785398, 1.570796]
Rank 2 executou 1024 trapezios no intervalo [1.570796, 2.356194]
Rank 3 executou 1024 trapezios no intervalo [2.356194, 3.141593]
Rank 0 executou 1024 trapezios no intervalo [0.000000, 0.785398]
With n = 4096 trapezoids, integral of f on [0, 3.14159] = 1.999999901954288e+00
Tempo de execucao = 0.000053 segundos
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex2_trap$ echo "0 3.141592653589793 16384" | mpiexec -n 4 ./mpi_trap_generalized
Enter a, b, n: Rank 2 executou 4096 trapezios no intervalo [1.570796, 2.356194]
Rank 0 executou 4096 trapezios no intervalo [0.000000, 0.785398]
Rank 1 executou 4096 trapezios no intervalo [0.785398, 1.570796]
Rank 3 executou 4096 trapezios no intervalo [2.356194, 3.141593]
With n = 16384 trapezoids, integral of f on [0, 3.14159] = 1.999999993872146e+00
Tempo de execucao = 0.000142 segundos

Para o estudo empírico da convergência da regra do trapézio, o programa paralelo foi executado com um número fixo de quatro processos (p=4), variando o número de subintervalos n∈{64,256,1024,4096,16384}. A teoria do cálculo numérico estabelece que o erro global de integração da regra do trapézio possui um comportamento assintótico O(h2), o que significa que o erro decai de forma quadrática em relação ao tamanho do passo h. Para comprovar este comportamento, os valores absolutos do erro (∣Iaprox​−2∣) foram recolhidos e traçados num gráfico em escala log-log em função de h.

Num gráfico com ambos os eixos em escala logarítmica, uma relação da forma Erro=C⋅hk transforma-se numa reta cuja inclinação corresponde diretamente à ordem de convergência k. Ao aplicar uma regressão linear sobre os dados obtidos, a inclinação da reta de ajuste empírica foi de exatamente 2.0000. Este resultado atesta de forma inequívoca a convergência quadrática teórica, provando que o modelo de divisão de domínio e a coleta paralela via MPI não introduziram quaisquer anomalias matemáticas, preservando na íntegra as propriedades numéricas do algoritmo original.

exercicio 3

vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex3_psum$ mpiexec -n 4 ./mpi_psum
Soma Serial = 4.9998874970e+06
Soma Global = 4.9998874970e+06
Erro Relativo = 3.9488965689e-14
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex3_psum$ mpiexec -n 2 ./mpi_
psum
Soma Serial = 4.9984512878e+06
Soma Global = 4.9984512878e+06
Erro Relativo = 5.7573568088e-14
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex3_psum$ mpiexec -n 1 ./mpi_
psum
Soma Serial = 4.9996657464e+06
Soma Global = 4.9996657464e+06
Erro Relativo = 0.0000000000e+00
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex3_psum$ mpiexec -n 8 --oversubscribe ./mpi_psum
Soma Serial = 5.0002277570e+06
Soma Global = 5.0002277570e+06
Erro Relativo = 7.8227532894e-15

Neste exercício, implementei a soma paralela de um vetor com 10 milhões de elementos gerados aleatoriamente. O programa dividiu corretamente a carga de trabalho e o resultado final da soma global aproximou-se perfeitamente do gabarito calculado serialmente. A presença de um erro relativo minúsculo (na ordem de precisão da máquina) já era esperada e não representa um defeito no código; este leve desvio ocorre simplesmente porque a aritmética de ponto flutuante computacional não é estritamente associativa, de modo que somar os blocos em paralelo altera os arredondamentos nos bits menos significativos em comparação com a soma sequencial contínua.

Para construir essa comunicação usando apenas as primitivas ponto-a-ponto (Aula 5), precisei escrever uma lógica extensa e manual. O processo 0 foi encarregado de executar um laço for iterando sobre cada trabalhador com chamadas de MPI_Send para distribuir os dados, enquanto os trabalhadores aguardavam em MPI_Recv. Após o cálculo local, um segundo ciclo foi necessário para que a raiz fizesse o caminho inverso, coletando as respostas uma a uma. Essa abordagem exigiu cerca de 18 a 20 linhas apenas dedicadas ao controle de índices, deslocamentos de memória (offsets) e divisões de if/else entre o mestre e os trabalhadores.

A comparação com o paradigma de comunicação coletiva (Aula 6) evidencia o salto em produtividade e eficiência. Todo o bloco de código do laço de distribuição seria substituído por uma única chamada à função MPI_Scatter. Analogamente, o laço de coleta com o somatório final colapsaria em uma única instrução de MPI_Reduce. Além da redução drástica na contagem de linhas e na superfície para bugs (como erros de ponteiros ou desencontros de tags), as rotinas coletivas executam algoritmos em formato de árvore subjacentes no hardware, trocando a latência linear O(p) da minha implementação manual por um custo assintótico logarítmico O(logp).



exercicio 4: 

4.a 

vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex4_coletivas$ mpiexec -n 4 .
/hello_gather
Greetings from process 0 of 4!
Greetings from process 1 of 4!
Greetings from process 2 of 4!
Greetings from process 3 of 4!

Neste exercício, substituí a topologia de comunicação em estrela (baseada em funções ponto-a-ponto) por uma rotina coletiva. Cada processo gerou sua própria string de saudação em um buffer local e, em vez de enviar as mensagens através de um laço de repetição com MPI_Send e MPI_Recv, utilizei a função MPI_Gather. Como resultado, a saída impressa no terminal apresentou todas as saudações perfeitamente ordenadas, do processo 0 ao último processo do comunicador.

Para que a coleta funcionasse corretamente em linguagem C, o processo raiz (rank 0) alocou dinamicamente um único vetor contíguo na memória, com tamanho suficiente para armazenar todas as strings simultaneamente (comm_sz * MAX_STRING). Durante a execução do MPI_Gather, o próprio ambiente MPI encarregou-se de agrupar as mensagens na rede e inseri-las de forma sequencial no vetor do processo 0. A impressão dos dados foi feita varrendo este vetor com saltos precisos de memória (offset), acessando o início de cada string através do índice i * MAX_STRING.

A principal vantagem observada ao abandonar a coleta manual foi a simplificação estrutural do código e a melhoria assintótica de comunicação. O laço de MPI_Recv utilizado nas aulas anteriores possui custo linear O(p), estrangulando o tráfego no processo raiz. Ao delegar essa tarefa para a rotina MPI_Gather, a biblioteca implementa algoritmos de árvore otimizados sob a abstração da função, melhorando o aproveitamento da rede e garantindo a ordenação determinística das mensagens pelo rank, sem a necessidade de gerenciar tags ou criar gargalos sequenciais.

4.b 

vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex4_coletivas$ mpiexec -n 2 ./minmax
--- Resultado Serial ---
Min = 0.0000000894 | Max = 0.9999999483
--- Resultado MPI ---
Min = 0.0000000894 | Max = 0.9999999483
vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex4_coletivas$ mpiexec -n 4 .
/minmax
--- Resultado Serial ---
Min = 0.0000000554 | Max = 0.9999999497
--- Resultado MPI ---
Min = 0.0000000554 | Max = 0.9999999497

O objetivo deste exercício foi substituir completamente o envio e a coleta manual de dados por rotinas coletivas otimizadas, focando na busca dos valores extremos de um vetor de grandes dimensões (N=107 doubles). Após o processo raiz inicializar o vetor global e computar os extremos de forma estritamente sequencial (para obtenção do gabarito), a distribuição da carga de trabalho foi realizada através de uma única chamada à função MPI_Scatter. Esta rotina fatiou a matriz e entregou a cada processo do comunicador um bloco perfeitamente dimensionado contendo N/p elementos, eliminando a necessidade de laços de MPI_Send e controle de offsets.

Com os dados em mãos, cada processo calculou seus próprios candidatos a mínimo e máximo locais de forma paralela e assíncrona. A grande vantagem computacional da Aula 6 refletiu-se na etapa de junção destes dados. Utilizando o MPI_Reduce associado aos operadores pré-definidos MPI_MIN e MPI_MAX, foi possível encontrar os limites globais do vetor sem escrever nenhuma lógica explícita de comparação ou ciclos de MPI_Recv no processo 0.

A saída do programa atestou a corretude matemática da abordagem paralela, uma vez que o resultado extraído pelas reduções do MPI perfez uma correspondência exata com o gabarito computado serialmente. Conceitualmente, a transição para funções coletivas como MPI_Scatter e MPI_Reduce é vital para o desempenho em supercomputadores. Ao delegar o fluxo para a biblioteca, o padrão de comunicação deixa de ser um "afunilamento" sequencial em estrela e passa a utilizar topologias internas em formato de árvore lógica, reduzindo o custo de tempo assintótico na rede de O(p) para O(logp).

exercicio 5: 

vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex5_vecadd$ make run
mpicc -O2 -Wall -o mpi_vecadd_gather mpi_vecadd_gather.c -lm
mpicc -O2 -Wall -o mpi_vecadd_allgather mpi_vecadd_allgather.c -lm
mpiexec -n 4 ./mpi_vecadd_gather
z = x + y:
  z[ 0] = 0
  z[ 1] = 3
  z[ 2] = 6
  z[ 3] = 9
  z[ 4] = 12
  z[ 5] = 15
  z[ 6] = 18
  z[ 7] = 21
  z[ 8] = 24
  z[ 9] = 27
  z[10] = 30
  z[11] = 33
  z[12] = 36
  z[13] = 39
  z[14] = 42
  z[15] = 45
mpiexec -n 4 ./mpi_vecadd_allgather
Rank 3 confirmando o recebimento completo. z = x + y:
  z[ 0] = 0
  z[ 1] = 3
  z[ 2] = 6
  z[ 3] = 9
  z[ 4] = 12
  z[ 5] = 15
  z[ 6] = 18
  z[ 7] = 21
  z[ 8] = 24
  z[ 9] = 27
  z[10] = 30
  z[11] = 33
  z[12] = 36
  z[13] = 39
  z[14] = 42
  z[15] = 45

A versão com MPI_Gather é suficiente quando o resultado paralelo tem como único objetivo a saída de dados pelo processo raiz. No entanto, o MPI_Allgather torna-se essencial em algoritmos onde o resultado global de uma etapa é o input obrigatório para todos os processos na etapa seguinte. Um exemplo é o produto matriz-vetor paralelo em métodos iterativos (onde a matriz é particionada por blocos de linhas). Para que cada processo consiga multiplicar as suas linhas locais, ele necessita do vetor multiplicador completo. Utilizando o MPI_Allgather, os fragmentos locais do vetor são reconstruídos e replicados simultaneamente na memória de cada nó, permitindo que o cálculo paralelo avance sem que a raiz precise realizar um gargalo de Broadcast.

exercicio 6: 

Neste exercício, comparei duas abordagens para o envio de uma estrutura de dados heterogênea (struct Student). A saída no terminal comprovou que o conteúdo transmitido e recebido por todos os processos é estritamente idêntico em ambas as versões. Isto confirma que a construção do tipo derivado com a função MPI_Type_create_struct mapeou a memória corretamente. Ao utilizar a macro offsetof, garanti que a leitura dos bytes saltasse qualquer padding (alinhamento de memória) invisível inserido pelo compilador, evitando a corrupção dos dados numéricos.

A principal métrica de avaliação solicitada foi a diferença no número de chamadas de comunicação. Na abordagem sequencial ingênua (student_three_bcasts), fui obrigada a invocar a rotina MPI_Bcast três vezes distintas para transferir um único aluno (uma chamada para o vetor de char, uma para o double e outra para o int). Em contraste, na abordagem otimizada, após o "mapa" do tipo derivado ser registrado no sistema (via MPI_Type_commit), a transmissão de toda a estrutura foi resolvida com apenas uma única chamada de MPI_Bcast.

Essa redução de 3 chamadas para 1 dita o ganho de desempenho em ambientes distribuídos. Como cada invocação de uma rotina de comunicação cobra um "pedágio" fixo de latência da rede (abertura do canal, criação de cabeçalhos de mensagem e roteamento no cluster), a abordagem dos três broadcasts paga essa latência três vezes. Ao empacotar variáveis heterogêneas num envelope único através do tipo derivado, a latência de rede é paga apenas uma vez, otimizando consideravelmente o tráfego, especialmente se esta rotina estiver inserida num laço iterativo de milhares de repetições.


vivian@vivian-ubuntu:~/CAD_20261/T1_souza_vivian/ex6_derived$ make run
mpicc -O2 -Wall -o student_struct student_struct.c -lm
mpicc -O2 -Wall -o student_three_bcasts student_three_bcasts.c -lm
mpiexec -n 4 ./student_struct
[Struct Bcast] Rank 0 recebeu -> Nome: Vivian Souza | Nota: 10.0 | ID: 123456789
[Struct Bcast] Rank 1 recebeu -> Nome: Vivian Souza | Nota: 10.0 | ID: 123456789
[Struct Bcast] Rank 2 recebeu -> Nome: Vivian Souza | Nota: 10.0 | ID: 123456789
[Struct Bcast] Rank 3 recebeu -> Nome: Vivian Souza | Nota: 10.0 | ID: 123456789
mpiexec -n 4 ./student_three_bcasts
[3 Bcasts] Rank 0 recebeu -> Nome: Vivian Souza | Nota: 10.0 | ID: 123456789
[3 Bcasts] Rank 1 recebeu -> Nome: Vivian Souza | Nota: 10.0 | ID: 123456789
[3 Bcasts] Rank 2 recebeu -> Nome: Vivian Souza | Nota: 10.0 | ID: 123456789
[3 Bcasts] Rank 3 recebeu -> Nome: Vivian Souza | Nota: 10.0 | ID: 123456789














