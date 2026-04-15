# Especificação da Linguagem Proc — v1.52

> **Disciplina:** Fundamentos de Compiladores — 2024.2  
> **Instituição:** Universidade do Estado da Bahia – UNEB / Colegiado de Sistemas de Informação  
> **Professor:** Antonio Atta

---

## Notação BNF Estendida

Nas regras léxicas e sintáticas descritas abaixo, os caracteres da notação BNF estão identificados conforme a seguinte convenção:

| Símbolo | Significado |
|---|---|
| `a \| b` | Alternativas — "a ou b" |
| `[ a ]` | Ocorrência **opcional** — equivale a `a \| ε` |
| `{ a }` | **Repetição** — equivale a `ε \| a \| aa \| aaa \| ...` |
| `(a \| b \| c)` | **Obrigatoriedade** de escolha de a, b ou c |

> `ε` refere-se à cadeia vazia.

---

## 1. Regras Léxicas

```
letra    ::= a | b | ... | z | A | B | ... | Z

digito   ::= 0 | 1 | ... | 9

id       ::= {_} letra { letra | digito | _ }

intcon   ::= digito { digito }

realcon  ::= intcon.intcon

charcon  ::= 'ch' | '\n' | '\0'
             onde ch denota qualquer caractere imprimível da tabela ASCII,
             como especificado pela função isprint() da linguagem C,
             diferente de \ (barra invertida) e ' (apóstrofo).

stringcon ::= "{ch}"
              onde ch denota qualquer caractere imprimível da tabela ASCII,
              como especificado pela função isprint() da linguagem C,
              diferente de " (aspas) e do caractere newline.

idconst  ::= indica a ocorrência de uma constante inteira declarada e
             inicializada previamente com "const int"

idproc   ::= indica a ocorrência de um nome de procedimento, exceto init
```

---

## 2. Regras Sintáticas

> Símbolos **não-terminais** são apresentados em *itálico*; símbolos **terminais** são apresentados em negrito.  
> Declarações e comandos devem estar estruturalmente completos em uma única linha.

### comentário
Comentários iniciam sempre com o sufixo `//` (dupla barra comum) e se estendem até a ocorrência do caractere de fim de linha.

### 2.1 Regras de Produção da Gramática

```
prog ::= {decl_list_var} {decl_def_proc}

decl_list_var ::= [const] tipo decl_var { , decl_var }

decl_def_proc ::= prot idproc ( [[&] tipo { [ ] } { , [&] tipo { [ ] } }] ) |
                  def ( init | idproc ( [ [&] tipo id1 { [( intcon | idconst )] }
                        { , [&] tipo id2 { [( intcon | idconst )] } }] ) )
                  {decl_list_var} { cmd } endp

decl_var ::= id [ = ( intcon | realcon | charcon ) ] |
             id {[ intcon | idconst ]} [ = { (intcon | realcon | charcon)
             {, (intcon | realcon | charcon) } } ]

tipo ::= char | int | real | bool

cmd ::= do idproc ( [ expr { , expr }] )
      | while (expr) { cmd } endw
      | var id from expr (to | dt) expr [ by ( intcon | idconst ) ] { cmd } endv
      | if ( expr ) { cmd } { elif ( expr ) { cmd } } [ else { cmd } ] endi
      | atrib
      | getout
      | getint id
      | getreal id
      | getchar id
      | getstr id
      | putint ( id | intcon )
      | putreal ( id | realcon )
      | putchar ( id | charcon )
      | putstr ( id | stringcon )

atrib ::= id { [ expr ] } = expr

expr      ::= expr_simp [ op_rel expr_simp ]

expr_simp ::= [+ | –] termo { (+ | – | ||) termo }

termo     ::= fator { (* | / | &&) fator }

fator     ::= id { [ expr ] } | intcon | realcon | charcon | ( expr ) | ! fator

op_rel    ::= == | != | <= | < | >= | >
```

### 2.2 Associatividade e Precedência de Operadores

> A precedência **diminui** da primeira para a última linha da tabela.

| Operador | Associatividade |
|---|---|
| `!`, `+`, `–` (unário) | direita para esquerda |
| `*`, `/` | esquerda para direita |
| `+`, `–` (binário) | esquerda para direita |
| `<`, `<=`, `>`, `>=` | esquerda para direita |
| `==`, `!=` | esquerda para direita |
| `&&` | esquerda para direita |
| `\|\|` | esquerda para direita |

---

## 3. Regras Semânticas

### 3.1 Declarações

1. Um identificador pode ser declarado no máximo **uma vez** como global e no máximo **uma vez** como local em um procedimento particular. Um mesmo identificador pode aparecer como local em vários procedimentos distintos. O escopo local tem precedência sobre o global enquanto o procedimento estiver em execução.

2. Declarações de identificadores podem ocorrer globalmente segundo a ordem estabelecida na gramática, mas a referência a qualquer elemento só pode ser feita **após** a sua declaração.

3. A declaração de todos os identificadores locais deve ocorrer **antes de qualquer comando**, conforme indicado na gramática.

4. No escopo global, um procedimento pode ter no máximo um nome e uma assinatura associada (protótipo); um procedimento pode ser **definido no máximo uma vez**. A linguagem não aceita procedimentos declarados e definidos localmente (no escopo interno de outro procedimento).

5. A declaração do protótipo de um procedimento é **opcional**, mas caso ocorra deve **anteceder** à sua definição.

6. Se um procedimento for usado (chamado) no corpo de comando de outro procedimento, a declaração do protótipo do procedimento chamado é **obrigatória** e deve ocorrer antes da definição do procedimento chamador.

7. Quando um procedimento não possuir um protótipo declarado, a sua assinatura corresponderá ao que é declarado na sua primeira linha de definição.

8. Os tipos dos parâmetros formais declarados na definição do procedimento devem ser iguais, **em número, tipificação e ordem**, com os parâmetros presentes no protótipo, se declarado.

9. O procedimento principal (`init`) é o único que **não deve possuir protótipo** associado.

10. Procedimentos **não possuem valor de retorno**; para retornar valores, deve-se usar parâmetros passados por referência (tipo precedido por `&`).

11. Um identificador pode aparecer **no máximo uma vez** na lista dos parâmetros formais na definição de um procedimento.

12. Os parâmetros formais de um procedimento possuem **escopo local** a esse procedimento.

13. A execução de todo procedimento (exceto `init`) deve ser feita **exclusivamente via comando `do`**. O procedimento `init` é o ponto de entrada de execução do programa.

14. Programas sem procedimento `init` podem ser compilados, mas não executados — funcionam como **bibliotecas de procedimentos**.

15. Nesta versão, a linguagem **não suporta tipos apontadores**.

16. O uso de chaves `{}` no lado direito de `=` em `decl_var` **só é permitido** (e deve ser sempre usado) na inicialização de arrays. Para variáveis escalares, o uso de chaves representa **erro semântico**.

17. Outras regras semânticas não tratadas neste documento podem existir e devem ser elucidadas à medida que questionamentos forem surgindo ao longo do desenvolvimento do compilador.

---

### 3.2 Requisitos de Consistência de Tipos

Variáveis devem ser declaradas antes de serem usadas. A **compatibilidade de tipos** é definida como:

1. `int` é compatível com `int`; `char` é compatível com `char`.
2. `int` é compatível com `char`, e vice-versa.
3. O tipo implícito de uma **expressão relacional** (ex.: `a >= b`) é `bool`, que é compatível com `int`. Um valor `0` indica falso lógico; qualquer inteiro diferente de zero indica verdadeiro lógico.
4. Qualquer par de tipos não coberto pelas regras acima **não é compatível**.

#### 3.2.1 Declaração e Definição de Procedimentos

1. Procedimentos **só podem ser executados via comando `do`**; não podem ocorrer como parte de uma expressão.
2. O marcador `&` antes do tipo indica **passagem por referência**. Sem `&`, a passagem é **por valor (cópia)**. O marcador `&` só é válido na assinatura/definição — sua ocorrência na chamada (`do`) é **inválida**.
3. Procedimentos podem ter variáveis locais. Os espaços de memória são alocados e liberados a cada execução.
4. Um procedimento **não pode** ser declarado ou definido internamente a outro procedimento.

#### 3.2.2 Expressões

O tipo de uma expressão `expr` é estabelecido como:

1. Se `expr` é uma **constante inteira**, seu tipo é `int`.
2. Se `expr` é um **identificador**, o tipo de `expr` é o tipo daquele identificador.
3. Se `expr` é `e1 + e2`, `e1 - e2`, `e1 * e2`, `e1 / e2` ou `-e1`, o tipo é compatível com os tipos dos operandos, restrito a `int`, `char` e `real`. Tipos incompatíveis geram **erro**.
4. Se `expr` é `e1 >= e2`, `e1 <= e2`, `e1 > e2`, `e1 < e2`, `e1 == e2` ou `e1 != e2`, o tipo é `bool`.
5. Se `expr` é `e1 && e2`, `e1 || e2` ou `!e1`, o tipo é `bool`.

**Regras adicionais de checagem de tipos:**

1. Cada argumento passado em uma execução de procedimento deve ser **compatível** com o parâmetro formal correspondente.
2. Operandos de `+`, `-`, `*`, `/`, `<=`, `>=`, `<`, `>`, `==`, `!=` devem ser compatíveis com `int`, `char` ou `real`. Operandos de `&&`, `||`, `!` devem ser compatíveis com `bool`.
3. Uma expressão usada como **índice de array** `[]` deve ser do tipo `int`.

#### 3.2.3 Comandos

1. Apenas variáveis dos tipos básicos (`int`, `char`, `real`, `bool`) podem receber atribuições. O tipo do lado direito deve ser compatível com o tipo do lado esquerdo.
2. A expressão condicional em `while`, `if` e `elif` deve ser do tipo `bool`. Se falsa na primeira avaliação, os comandos internos **não são executados**.
3. Em um comando `var`: se usada a cláusula `to`, `expr1` deve ser **menor** que `expr2`; se usada `dt`, `expr1` deve ser **maior** que `expr2`. Caso contrário, os comandos internos não são executados.

---

## 4. Características Operacionais

### 4.1 Dados

#### 4.1.1 Escalares

Variáveis do tipo escalar (`int`, `real`, `char`, `bool`) ocupam **uma posição de memória** na máquina virtual do compilador. Valores do tipo `char` são sinalizados; conversões para `int` **estendem o sinal**.

#### 4.1.2 Estruturados (Arrays)

A linguagem suporta arrays **unidimensionais** (vetores) e **bidimensionais** (matrizes). Elementos são acessíveis via operador de indexação `[]`. Internamente, arrays são armazenados em **posições contíguas de memória**; matrizes são armazenadas por linha.

#### 4.1.3 Constantes Cadeias (Strings)

Uma string `"a1a2a3…an"` é um vetor de caracteres com `n+1` elementos — os `n` caracteres da cadeia mais o caractere `'\0'` (NULL) ao final.

#### 4.1.4 Inicialização

- Identificadores **constantes** (`const`) devem ser explicitamente inicializados na sua declaração. Todos os identificadores de uma mesma linha `const` são constantes e devem ser individualmente inicializados (ex.: `const int x = 10, y = 15`).
- Identificadores **não constantes** não inicializados explicitamente recebem valores padrão:
  - `int` → `0`
  - `real` → `0.0`
  - `char` → `'\0'`
  - `bool` → `0` (falso)
- Arrays podem ser inicializados com `{ valor1, valor2, ... }`. Se o número de valores for **inferior** ao número de elementos, o último valor é repetido para os demais. Se for **superior**, um **erro** é emitido. Em matrizes, o preenchimento ocorre por linha.

---

### 4.2 Expressões

#### 4.2.1 Ordem de Avaliação

- **Aritméticas:** os operandos são calculados antes da expressão. Multiplicações e divisões precedem adições e subtrações, salvo uso de parênteses.
- **Booleanas:** operadores relacionais (`>=`, `>`, `<=`, `<`, `==`, `!=`) e lógicos (`&&`, `||`, `!`) são avaliados segundo a técnica do **circuito mais curto** (*short-circuit evaluation*).

#### 4.2.2 Conversão de Tipos

Se um objeto do tipo `char` faz parte de uma expressão, seu valor é convertido (com extensão de sinal) para `int` antes da avaliação.

---

### 4.3 Comandos de Atribuição

A expressão do lado direito de uma atribuição (`=`) é avaliada respeitando as **regras de precedência dos operadores** estabelecidas na gramática.

---

### 4.4 Procedimentos

#### 4.4.1 Execução

A execução de um procedimento é sempre iniciada pelo comando **`do`**, que executa o procedimento **uma única vez**.

#### 4.4.2 Avaliação de Argumentos

Os argumentos são avaliados na **ordem de ocorrência** (esquerda para direita), antes do início da execução do procedimento.

#### 4.4.3 Passagem de Parâmetros

- **Por valor (padrão):** uma cópia do argumento é passada ao procedimento.
- **Por referência (`&`):** alterações no parâmetro dentro do procedimento refletem no escopo externo.
- **Constantes** não podem ser associadas a parâmetros por referência — apenas **identificadores variáveis**.
- Parâmetros do tipo **array** são sempre passados **por referência** (não devem ser declarados com `&`). Suas dimensões máximas devem ser declaradas no protótipo e na definição. Na chamada via `do`, apenas o **nome do array** (sem dimensões) é especificado.

#### 4.4.4 Fim da Execução

Um procedimento encerra sua execução ao final regular do bloco ou pela ocorrência do comando **`getout`**. O `getout` ao final do procedimento é **opcional** — está implícito.

---

### 4.5 Execução do Programa

A execução do programa inicia no procedimento especial **`init`**, caso exista, que corresponde ao **ponto de entrada** de execução do programa.

---

*Especificação da Linguagem Proc v1.52 — UNEB / Fundamentos de Compiladores 2024.2*