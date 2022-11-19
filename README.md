# Sistema Solar

Este repositório se trata de um fork da biblioteca ABCG implementada pelo Dr. Harlen Batagelo, aqui foi implementado uma simulação do sistema solar para a fixação dos conceitos aprendidos na matéria MCTA008-17 Computação Gráfica. 
## Autores:
Nome: Beatriz Favini Chicaroni  
RA 11201721608

Nome: Thais Amorim Souza
RA 11201810655

### Descrição do projeto

- O nosso sistema solar contem os seguintes elementos:
  * [X] Sol;
  * [X] Mercúrio;
  * [X] Vênus;
  * [X] Terra;
  * [X] Marte;
  * [X] Júpiter;
  * [X] Saturno;
  * [X] Urano;
  * [X] Netuno;
  * [] Luas;
  * [] Aneis de Saturno;
  * [] Universo Estrelado;
  * [] Asteroides;

- Este projeto tem o proposito de simular:
  * [X] O comportamento de rotação dos planetas em relação ao Sol;
  * [] O comportamento de rotação das luas dos planetas;
  * [X] A ideia de escala entre os tamanhos dos elementos;
  * [X] Mostrar graficamente os elementos do Sistema Solar em uma perspectiva 3D;

- Para a parte da câmera virtual deste projeto, foi utilizada a câmera implementada no projeto ![LookAt](https://hbatagelo.github.io/cg/lookat.html);

- Para simular o efeito de rotação dos planetas em relação ao Sol utilizamos dos conceitos de ![Movimento Circular Uniforme](https://wp.ufpel.edu.br/diehl/files/2018/08/FGA_aula6.pdf) aplicando uma velocidade angular constante parametrizada para cada planeta, e o conceito de ![Coordenadas Polares](http://wwwp.fc.unesp.br/~mauri/Down/Polares.pdf) para obter as coordenadas cartesianas na função update().








