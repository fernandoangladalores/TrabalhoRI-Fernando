#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include "Nodo.h"
#include <string.h>

class  Lista{
      private:
          NodoLista *primero;
          NodoLista *actual;
      public:
           Lista();
           void insertar_NodoLista(string,int);
           void elimiar_NodoLista(int);
           void actualizar_NodoLista(string,int,int);
           NodoLista *return_NodoLista(int);         //Retorna el NodoLista segun posicion
           int cant_NodoListas();
};

//--------------------------------------------//
Lista::Lista()
 { primero=NULL;
   actual=NULL;
 }
//---------------------------------------------//
void Lista::insertar_NodoLista(string id,int cant) //lista invertida
 {
    primero = new NodoLista(id,cant,actual);
    actual = primero;
 }
//------------------------------------------------//

int Lista::cant_NodoListas()
{  int cont=0;
   NodoLista *temp;
   temp=primero;
   while(temp!=NULL){
                     cont++;
                     temp=temp->prox;
                    }
   return cont;
}

//-----------------------------------------------------------//
NodoLista* Lista::return_NodoLista(int pos)
{   int cont=0;
    actual=primero;
    while (cont!=pos)
    {
    cont++;
    actual=actual->prox;
    }
   if(actual!=NULL)
    return actual;
}

//--------------------------------------------------------------//

void Lista::elimiar_NodoLista(int pos )
{   NodoLista *anterior;
    NodoLista *temp=primero;

   if(temp==NULL)
                {}


   else{  int cont=0;
         while((temp->prox!=NULL)&& (pos!=cont))
            {
              anterior=temp;
               temp=temp->prox;
               cont++;
             }

          if(temp!=NULL)
          {
             if(temp==primero)
                              { primero=temp->prox;
                                delete(temp);
                              }

                         else{ anterior->prox=temp->prox;
                               delete(temp);
                             }

       }
          }
}


void Lista::actualizar_NodoLista(string id, int cant, int pos )
{
  actual = return_NodoLista(pos);
  actual->set_id(id);
  actual->set_cant(cant);
}



#endif // LISTA_H_INCLUDED
