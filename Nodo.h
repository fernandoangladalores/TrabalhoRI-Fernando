#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED
#include <string.h>
#include <string>

using namespace std;

 class  NodoLista{
       private:
              string id;     ///PN de cada artigo
               int  cant;    ///Frecuencia de repeticao
              NodoLista *prox;
       public:
         NodoLista();
         NodoLista(string,int,NodoLista*);
    void set_id(string i)  { id=i;}
    void set_cant(int c)  { cant=c;}
    void aumeta_cant()  { cant++;}
  string get_id()   {return id;}
     int get_cant()  {return cant;}
      friend class Lista;
 };

 NodoLista::NodoLista(){
    id="";
    cant=0;
    prox=NULL;
  }
//---------------------------------------------//
 NodoLista::NodoLista(string i,int c,NodoLista *p){
      id=i;
      cant=c;
      prox=p;
  }
 //------------------------------------------------//

#endif // NODO_H_INCLUDED
