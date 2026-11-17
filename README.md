# Cyber Sudoku

Cyber Sudoku é um jogo de Sudoku desenvolvido em C utilizando a biblioteca Raylib, como projeto da disciplina de Programação Imperativa e Funcional (PIF) da CESAR School.

O jogo tem uma temática de invasão de sistemas, onde o jogador precisa completar os tabuleiros utilizando símbolos no lugar dos números.

## Como funciona o jogo

O objetivo é completar o Sudoku sem repetir símbolos na mesma linha, coluna ou bloco.

O jogo possui duas fases:

- **Fase 1:** Sudoku 4x4, com 4 símbolos.
- **Fase 2:** Sudoku 9x9, com 9 símbolos.

O jogador recebe 100 pontos por acerto e mais 1000 pontos ao concluir cada fase. Quando erra, a pontuação é reduzida pela metade.

Também existem 3 dicas disponíveis por partida, que revelam o símbolo correto de uma célula sem dar pontos.

Além disso, o jogo possui cronômetro, histórico de jogadas, pausa e ranking com os 10 melhores resultados.

## Como instalar e executar

O projeto foi desenvolvido e testado no Ubuntu através do WSL (Windows Subsystem for Linux), utilizando o Windows 11.

Para executar o jogo, é necessário ter um ambiente Linux com GCC, Make e Raylib instalados.

### 1. Instalando o WSL e o Ubuntu

Caso esteja utilizando Windows e ainda não tenha o WSL instalado, abra o PowerShell como administrador e execute:

```powershell
wsl --install -d Ubuntu
```

Depois da instalação, reinicie o computador caso seja necessário.

Abra o Ubuntu e faça a configuração inicial, criando um usuário e uma senha.

Se já tiver o Ubuntu instalado no WSL, pode pular essa etapa.

### 2. Instalando as ferramentas necessárias

No terminal do Ubuntu, execute:

```bash
sudo apt update
sudo apt install -y build-essential git cmake pkg-config
```

Esses comandos instalam as ferramentas necessárias para compilar o projeto e instalar a Raylib.

### 3. Instalando as dependências da Raylib

Execute:

```bash
sudo apt install -y libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

Essas bibliotecas são necessárias para o funcionamento dos recursos gráficos, áudio e janelas utilizados pela Raylib.

### 4. Instalando a Raylib

Primeiro, clone o repositório oficial da Raylib:

```bash
git clone https://github.com/raysan5/raylib.git
```

Entre na pasta:

```bash
cd raylib
```

Crie uma pasta para a compilação:

```bash
mkdir build
cd build
```

Depois, execute:

```bash
cmake -DBUILD_EXAMPLES=OFF ..
make
```

Por último, instale a biblioteca:

```bash
sudo make install
sudo ldconfig
```

Depois de finalizar, volte para a pasta inicial:

```bash
cd ~
```

### 5. Clonando o projeto

Agora, clone o repositório do Cyber Sudoku:

```bash
git clone https://github.com/Ranilton10/CyberSudoku-PIF.git
```

Entre na pasta do projeto:

```bash
cd CyberSudoku-PIF
```

### 6. Compilando e executando o jogo

Dentro da pasta do projeto, execute:

```bash
make run
```

Esse comando compila o código e inicia o jogo.

Também é possível utilizar os comandos separadamente.

**Para compilar:**

```bash
make
```

**Para executar depois de compilado:**

```bash
./cyber_sudoku
```

**Para apagar o executável gerado:**

```bash
make clean
```

É importante executar esses comandos dentro da pasta principal do projeto, pois o jogo precisa acessar os arquivos de imagens, música e ranking.

### 7. Observações para quem utiliza Windows

Como o projeto utiliza a Raylib no ambiente Linux, recomendamos executar pelo Ubuntu através do WSL.

No Windows 11, o WSL normalmente já permite abrir aplicações gráficas do Linux.

Caso o jogo não abra, tente atualizar o WSL pelo PowerShell:

```powershell
wsl --update
```

Depois, execute:

```powershell
wsl --shutdown
```

Abra novamente o Ubuntu, entre na pasta do projeto e tente executar `make run`.

## Vídeo do jogo

Link: https://youtu.be/JS6s7DBkLEM

## Integrantes

Nicolas Vitor, Gabriel Feitosa e Ranilton Araújo.
