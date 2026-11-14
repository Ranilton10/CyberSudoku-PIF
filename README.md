<h1 align = center>CyberSudoku</h1>
<h3> O CyberSudoku é um jogo casual, onde você é um Hacker que invade um sistema resolvendo sudokus</h3>
<div align = center>
  <h5> (Foto do jogo será inserida aqui em breve)</h5>
</div>
<h3>O objetivo do jogo é preencher todas as posições com os símbolos certos</h3>
<h3> Esse projeto foi feito por estudantes de Ciências da computação do Segundo período da Cesar School.</h3>
<h2 align = center>Como Jogar💻</h2>
<p>O jogo possui uma grade de células, parcialmente preenchidas com símbolos diferentes </p>
<p>As células vazias devem ser preenchidas com as imagens corretas, respeitando as regras abaixo: </p>
<p><b> 1. Não pode ter 2 símbolos iguais na mesma linha </b></p>
<p><b> 2. Não pode ter 2 símbolos iguais na mesma coluna </b></p>
<p><b> 3. Não pode ter 2 símbolos iguais no mesmo bloco(3X3)</b></p>
<p>O jogo constitui de 2 fases e 2 dificuldades diferentes: A Fácil e a Difícil</p>
<p>Dificuldade Fácil: Possui 4 linhas e 4 colunas, totalizando 16 células para serem preenchidas com 4 símbolos diferentes</p>
<p>Dificuldade Difícil: Possui 9 linhas e 9 colunas, totalizando 81 células para serem preenchidas com 9 símbolos diferentes</p>
<p> O jogo encerra quando você completa todas as fases e invade o sistema</p>
<p><b> Para rodar o jogo escreva no terminar:</b></p>
<p>$env:PATH = "C:\raylib\w64devkit\bin;" + $env:PATH</p>
<p>gcc src/main.c src/menu.c src/jogo.c src/historico.c src/ranking.c -o cyber_sudoku.exe -Iinclude -Wall -Wextra -std=c11 -lraylib -lopengl32 -lgdi32 -lwinmm</p>
<p>.\cyber_sudoku.exe </p>
