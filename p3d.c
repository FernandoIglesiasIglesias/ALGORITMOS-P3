/*
AUTORES:
- Fernando Iglesias Iglesias - login: f.iglesias2
- Anxo Gallego Mato          - login: anxo.gallego
- Luís López García          - login: luis.lopez
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

struct nodo
{
    int elem;
    int num_repeticiones;
    struct nodo *izq, *der;
};
typedef struct nodo *posicion;
typedef struct nodo *arbol;

static struct nodo *crearnodo(int e);
arbol insertar(int e, arbol a);
arbol creararbol();
int esarbolvacio(arbol a);
posicion buscar(int x, arbol a);
arbol eliminararbol(arbol a);
posicion hijoizquierdo(arbol a);
posicion hijoderecho(arbol a);
int elemento(posicion pos);
int numerorepeticiones(posicion pos);
int altura(arbol a);
void visualizar(arbol a);
void test();
double microsegundos();
void inicializar_semilla();
void generar_vector_random(int v[], int n);
void calc_tiempos(double info_t_ins[8], double info_t_bus[8]);
void imprimir_tablas();

int main() {
    inicializar_semilla();
    test(); 
    imprimir_tablas();
}

void test() {
    int i, num, numreps; arbol a;
    printf("------ Test funciones ------\n");
    printf("Creamos un arbol nuevo\n");
    a=creararbol();
    visualizar(a);
    printf("La altura del arbol es: %d\n",altura(a));
    a=insertar(3,a); a=insertar(1,a); a=insertar(2,a);
    a=insertar(5,a); a=insertar(4,a); a=insertar(5,a);
    visualizar(a);
    printf("\nLa altura del arbol es: %d\n",altura(a));
    for (i=1; i<= 6; i++) {
        if (buscar(i,a)==NULL) {
            printf("busco %d y no encuentro nada\n",i);
        } else {
            num = buscar(i,a)->elem;
            numreps = buscar(i,a)->num_repeticiones;
            printf("encuentro %d y se repite %d veces\n",num,numreps);
        }
    }
    printf("Se elminan todos los nodos del arbol:\n");
    a=eliminararbol(a);
    visualizar(a);
    printf("La altura del arbol es: %d\n",altura(a));
}

//Funcion que calcula los tiempos de insertar y buscar
void calc_tiempos(double info_t_ins[8], double info_t_bus[8]) {
    int n, i, j=0; int inser[256000]; double ta,tb,t; arbol arb;
    arb = creararbol();
    for (n = 1000; n<= 256000; n=n*2) {
        generar_vector_random(inser,n);
        ta = microsegundos();
        for(i = 0; i < n; i++) {
            arb = insertar(inser[i],arb);
        }
        tb = microsegundos();
        t = tb - ta;
        info_t_ins[j] = t;
        
        generar_vector_random(inser,n);
        ta = microsegundos();
        for(i = 0; i < n; i++) {
            buscar(inser[i],arb);
        }
        tb = microsegundos();
        t = tb - ta;
        info_t_bus[j] = t;
        j++;
        arb = eliminararbol(arb);
    }
}

void imprimir_tablas() {
    int n, i=0, j=0; double t_ins[8]; double t_bus[8]; double ajs, sub, sobr;
    calc_tiempos(t_ins,t_bus);
    printf("\nTabla insercion n elementos: \n");
    printf("      n      t(n)      t(n)/f(n)     t(n)/g(n)     t(n)/h(n)\n");
    for(n = 1000; n <= 256000; n = n*2) {
        if ((t_ins[i] >= 500) && (t_bus[i] >= 500)) {
            sub=t_ins[i]/(pow(n,0.95));
            ajs=t_ins[i]/(pow(n,1.25)); 
            sobr=t_ins[i]/pow(n,1.5);
            printf("%7d%10.1f%14.6f%14.6f%14.6f\n",n,t_ins[i],sub,ajs,sobr);
        }
        i++;
    }
    printf("\nTabla busqueda n elementos: \n");
    printf("      n      t(n)      t(n)/f(n)     t(n)/g(n)     t(n)/h(n)\n");
    for(n = 1000; n <= 256000; n = n*2) {
        if ((t_bus[j] >= 500) && (t_ins[j] >= 500)) {
            sub=t_ins[j]/pow(n,0.95);
            ajs=t_ins[j]/(pow(n,1.25)); 
            sobr=t_ins[j]/pow(n,1.5);
            printf("%7d%10.1f%14.6f%14.6f%14.6f\n",n,t_bus[j],sub,ajs,sobr);
        }
        j++;
    }
}


static struct nodo *crearnodo(int e) {
    struct nodo *p = malloc(sizeof(struct nodo));
    if (p == NULL) {
        printf("memoria agotada\n"); exit(EXIT_FAILURE);
    }
    p->elem = e;
    p->num_repeticiones = 1;
    p->izq = NULL;
    p->der = NULL;
    return p;
}
arbol insertar(int e, arbol a) {
    if (a == NULL) {
        return crearnodo(e);
    } else if (e < a->elem) {
        a->izq = insertar(e, a->izq);
    } else if (e > a->elem) {
        a->der = insertar(e, a->der);
    } else {
        a->num_repeticiones++;
    }
    return a;
}
arbol creararbol() { //crea un arbol vacio
    arbol a;
    return a=NULL;
}
int esarbolvacio(arbol a) {
    if (a==NULL) {
        return 1;
    } else {
        return -1;
    }
}
posicion buscar(int x, arbol a) { //bucar un elemento x en el arbol
    if (esarbolvacio(a)==1) {
        return NULL;
    } else if (x == a->elem) {
        return a;
    } else if (x < a->elem) {
        return (buscar(x,a->izq));
    } else {
        return (buscar(x,a->der));
    }
}
arbol eliminararbol(arbol a) {
    arbol temp=NULL;
    if (esarbolvacio(a)==1) {
        return a;
    } else {
        if (a->izq!=NULL) {
            eliminararbol(a->izq);
            temp=a; a=NULL;
            free(temp);
        } else if (a->der!=NULL) {
            eliminararbol(a->der);
            temp=a; a=NULL;
            free(temp);
        } else {
            temp=a; a=NULL;
            free(temp);
        }
    }
    return a;
}
posicion hijoizquierdo(arbol a) {
    if (esarbolvacio(a)==1) {
        return NULL;
    } else {
        return (a->izq);
    }
}
posicion hijoderecho(arbol a) {
    if (esarbolvacio(a)==1) {
        return NULL;
    } else {
        return a->der;
    }
}
int elemento(posicion pos) {
    return (pos->elem);
}
int numerorepeticiones(posicion pos) {
    return(pos->num_repeticiones);
}
int max (int a, int b) {
    if (a >= b) return a;
    else return b;
}
int altura(arbol a) {
    if (esarbolvacio(a)==1) return -1;
    else {
        return (1 + max(altura(a->izq),altura(a->der)));
    }
}
void imprimir (arbol a) {
    if (esarbolvacio(a)==-1) {
        printf( " ( ");
        imprimir(a->izq);
        printf("%d",a->elem);
        imprimir(a->der);
        printf(" ) ");
    }
}
void visualizar(arbol a) {
    if (esarbolvacio(a)==1) {
        printf("arbol vacio: ().\n");
    } else {
        imprimir(a);
    }
}
double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}
void inicializar_semilla() {
    srand(time(NULL));
}
void generar_vector_random(int v[], int n) {
    int i, m=2*n+1;
    for (i = 0; i < n; i++) {
        v[i]= (rand()%m) - n;
    }
}