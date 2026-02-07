## Descrição

Este programa em C lê uma imagem binária no formato **PBM (P1)**, identifica e rotula os objetos presentes utilizando componentes conexos (vizinhança 8) e calcula:

* Número total de objetos na imagem
* Número de objetos que possuem buracos internos

Um objeto é definido como um conjunto de pixels com valor `1`.
Um objeto possui buraco quando existe uma região de fundo completamente cercada pelo objeto.

---

## Requisitos

* GCC ou outro compilador C compatível

Verificar se o GCC está instalado:

```
gcc --version
```

---

## Compilação

No diretório onde está o arquivo `GenericStuff.c`, execute:

```
gcc -o programa GenericStuff.c
```

Isso irá gerar um executável chamado `programa`.

---

## Formato do Arquivo de Entrada

O programa espera uma imagem no formato **PBM ASCII (P1)**.

Exemplo de arquivo válido:

```
P1
5 4
0 1 0 0 1
1 1 0 1 0
0 0 1 1 0
0 1 0 0 0
```

Regras:

* Primeira linha deve ser `P1`
* Segunda linha deve conter: `<colunas> <linhas>`
* Apenas valores `0` e `1`
* Valores separados por espaço

---

## Execução

Após compilar, execute:

```
./programa imagem.pbm
```

Exemplo:

```
./programa exemplo.pbm
```

---

## Saída

O programa exibirá:

* Dimensões da imagem (com padding interno)
* Quantidade total de objetos
* Quantidade de objetos com buraco
