#include<iostream>
#include <cstdlib>
#include <random>
#include <ctime> 
#include<array>
#include<cmath>
using namespace std;

const int saldoInicialPorDefecto = 500;
const int min_card = 1;
const int max_card = 13;
const int manoDeCartas = 20;

typedef array<int, manoDeCartas>TMano;

void bienvenida(){
  cout<<"Hola, soy un programa que juega contra un gilipollas (Tú) al Black Jack"<<"\n"<<"Empiezas con 500$, seguro te los ventilas en 5 minutos por k eres gilipollas"<<"\n";
}

int declararApuesta(int saldoDelUsuario){
  cout<<"Cuánto vas a apostar"<<"\n";
  int miApuesta = 0;

  do{
    cin>>miApuesta;
    if(miApuesta <= 0){
      cout<<"Va a apostar en nulo tu puta madre"<<"\n";
    }
    if(miApuesta > saldoDelUsuario){
      cout<<"Pretendes acabar bajo un puente gilipollas?"<<"\n";
    }
    if(miApuesta == saldoDelUsuario){
      cout<<"Ya está haciendo all-In el ludópata gilipollas este de los huevos..."<<"\n";
    }
  }while(miApuesta <= 0 | miApuesta > saldoDelUsuario);

  return miApuesta;
}

void realizarApuesta(int&saldoDelUsuario, int miApuesta){
  saldoDelUsuario += -miApuesta;
}

void vaciarManos(TMano&miMano, TMano&manoCPU){
  for(int i = 0; i<manoDeCartas; i++){
    miMano[i] = 0;
    manoCPU[i] = 0;
  }
}

int sumMano(TMano mano){
  int suma = 0;
  for(int i = 0; i < manoDeCartas; i++){
    suma += mano[i];
  }
  return suma;
}

void sumarValorCarta(int valor, TMano&manoActual){
  int index = 0;
  bool freeSpace = false;
  
  while(!freeSpace && index < manoDeCartas){
    if(manoActual[index] == 0 && !freeSpace){
      manoActual[index] = valor;
      freeSpace = true;
    }

    index++;
  }
}

void repartirCarta(TMano&manoActual, bool isMe){
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> distrib(1, 13);
  int carta = distrib(gen);

  switch(carta){
    case 1:
      if (sumMano(manoActual) <= 10){
        sumarValorCarta(11, manoActual);
        if(isMe){cout<<"Obtuviste un As, te contará como 11 puntos"<<"\n";}
      }else{
        sumarValorCarta(1, manoActual);
        if(isMe){cout<<"Obtuviste un As, te contará como 1 punto"<<"\n";}
      }
      break;
    
    case 11:
      sumarValorCarta(10, manoActual);
      if(isMe){cout<<"Obtuviste un Jack, te contará como 10 puntos"<<"\n";}
      break;
    
    case 12:
      sumarValorCarta(10, manoActual);
      if(isMe){cout<<"Obtuviste una Reina, te contará como 10 puntos"<<"\n";}
      break;
    
    case 13:
      sumarValorCarta(10, manoActual);
      if(isMe){cout<<"Obtuviste un Rey, te contará como 10 puntos"<<"\n";}
      break;
    
    default:
      sumarValorCarta(carta, manoActual);
      if(isMe){cout<<"Obtuviste un "<<carta<<"\n";}
      break;
  }
  if(isMe){cout<<"LLevas "<<sumMano(manoActual)<<" Punto(s)"<<"\n";}
}

void repartirPrimerasCartas(TMano&miMano, TMano&manoCPU){
  cout<<"Repartimos las dos primeras cartas"<<"\n";

  for(int i = 0; i < 2; i++){
    repartirCarta(miMano, true);
    repartirCarta(manoCPU, false);
  }

}

int decidir(){
  int decision = -1;
  do{
    cout<<"Tu turno, que coño vas a hacer ahora?"<<"\n";
    cout<<"0 - Pedir carta"<<"\n";
    cout<<"1 - Quedarte como estás"<<"\n";
    cin>>decision;
  }while(decision < 0 | decision > 1);

  return decision;
}

void cursoDeRonda(TMano&miMano, TMano&manoCPU){
  int decisionUsuario = -1;
  bool stop = false;
  
  while(sumMano(miMano) < 21 && !stop){
    decisionUsuario = decidir();
    switch(decisionUsuario){
      case 0:
        repartirCarta(miMano, true);
        break;
      case 1:
        stop = true;
        break;
    }
  }

  if(sumMano(miMano) <= 21){
    cout<<"Me toca"<<"\n";
    if(sumMano(manoCPU) < 17){
      cout<<"Tengo menos de 17 puntos, robo carta"<<"\n";
      repartirCarta(manoCPU, false);
    }else{
      cout<<"Tengo más de 17 puntos, no robo carta"<<"\n";
    }
    cout<<"Tengo "<<sumMano(manoCPU)<<" punto(s)"<<"\n";
  }else{
    cout<<"Te pasaste de 21, tontopollas"<<"\n";
  }
}

int juicioFinal(TMano miMano, TMano manoCPU){
  int diferenciaUser = abs(sumMano(miMano) - 21);
  int diferenciaCPU = abs(sumMano(manoCPU) - 21);

  int veredicto = -1;

  if(sumMano(miMano) > 21 | (diferenciaUser > diferenciaCPU && sumMano(manoCPU) <= 21)){
    cout<<"Perdiste gilipollas, a joderse"<<"\n";
    veredicto = 0;
  }else if(sumMano(manoCPU) > 21 | diferenciaUser < diferenciaCPU){
    if(diferenciaUser == 0){
      cout<<"Te has hecho un BlackJack de putisima potra man"<<"\n";
    }else{
      cout<<"Ganaste de puta potra hermano, te llevas el doble"<<"\n";
    }
    veredicto = 2;
  }else{
    cout<<"Empate (recuperas tu apuesta pero sigues siendo gilipollas)"<<"\n";
    veredicto = 1;
  }

  return veredicto;
}

void jugarPartida(int&saldoDelUsuario, TMano&miMano, TMano&manoCPU){
  int apuesta;
  
  do{
    apuesta = declararApuesta(saldoDelUsuario);
    realizarApuesta(saldoDelUsuario, apuesta);
    vaciarManos(miMano, manoCPU);
    repartirPrimerasCartas(miMano, manoCPU);

    cursoDeRonda(miMano, manoCPU);
  
    saldoDelUsuario += apuesta * juicioFinal(miMano, manoCPU);

    cout<<"Ahora tienes "<<saldoDelUsuario<<"€"<<"\n";

  }while(saldoDelUsuario > 0);
}

void despedida(){
  cout<<"PERO QUE PUTO GILIPOLLAS TE HAS QUEDAO ARRUINAO XDDDDDDDDDDDDDDDD";
}

int main(){
  int saldoDelUsuario = saldoInicialPorDefecto;

  TMano miMano;
  TMano manoCPU;

  bienvenida();
  jugarPartida(saldoDelUsuario, miMano, manoCPU);
  despedida();

  return 0;
}