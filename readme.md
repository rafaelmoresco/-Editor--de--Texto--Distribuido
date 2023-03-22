## Editor de Texto Distribuido

Atividade prática para desenvolver programas com *Sockets* em C.
Foi realizado um servidor basico que armazena em memória temporária
uma matriz de caracteres, que pode ser editada linha por linha pelos
programas clientes.

# Executando

Para rodar o programa, é necessario inicialmente buildar os executavies:
```sh
gcc cliente.c -o cliente && gcc servidor.c -o servidor -lpthread
```
Em seguida, execute o programa servidor com:
```
./servidor
```
Em outro console, abra uma instancia do cliente com:
```sh
./cliente
```

# Comandos do Cliente
Escrever em uma linha desejada na matriz
```
add_line
```
Ler uma linha desejada da matriz
```
get_line
```
Encerrar a conexão
```
exit
```