
# Problema de los Productores y Consumidores
Alumno: Luisfelipe Rodrigo Mamani Arosquipa
Docente: Richart Smith Escobedo Quispe
Asignatura: Sistemas Operativos


**Tabla de contenidos**

[TOCM]

[TOC]

## Implementacion en c++
    
    #include "iostream"
    #include<thread>
    #include<mutex>
    #include<deque>
    #include <condition_variable>
    using namespace std;
    // g++ productores02.cpp -lpthread
    
    
    #define NUM_PRODUCTORES 2
    condition_variable cond;
    mutex mu;
    deque<int> buffer;
    //tamaño del bufer
    const unsigned int maxBufferSize = 10;
    //void productor (int k);
    
    //clase productor
    class Productor {
    	private:
    		thread t;
    		int k;
    		void produce()
    		{
    			//numero maximo que produciran los productores e ira disminuyendo hasta 0
    			int val=100;
    			while (val)
    			{
    				unique_lock<mutex> locker(mu);
    				cond.wait(locker, []()
    				  		{ return buffer.size() < maxBufferSize; });
    				//agregamos la produccion al buffer
    				buffer.push_back(val);
    				cout << "Productor " <<k<<" produce: "<< val << endl;
    				val--;
    				locker.unlock();
    				//notificamos al otro hilo que puede continuar
    				cond.notify_one();
    			}
    		}
    	public:
    		Productor(int id) {
    			k = id;
    			t = thread(&Productor::produce, this);
    			//t.join();
    		}
    		void join_thread(){
    			t.join();
    		}
    };
    
    //clase consumidor
    class Consumidor {
    	private:
    		thread t;
    		int k;
    		void consume() {
    			//los consumidores siempre estaran consumiendo
    
    			while (true)
    			{
    				unique_lock<mutex> locker(mu);
    				cond.wait(locker, []()
    				  		{ return buffer.size() > 0; });
    				//val es ultimo valor del buffer
    				int val = buffer.back();
    				//el consumidor consume el ultimo elemento
    				buffer.pop_back();
    				cout << "consumidor "<<k<<" consume:" << val << endl;
    				locker.unlock();
    				//notificamos a los otros hilos que pueden continuar
    				cond.notify_one();
    			}
    		}
    	public:
    		Consumidor(int id) {
    			k = id;
    			t = thread(&Consumidor::consume, this);
    			//t.join();
    		}
    		void join_thread(){
    			t.join();
    		}
    };
    
    int main(void)
    {
    	//creamos una lista de consumidores y productores
    	Productor* productores[NUM_PRODUCTORES];
    	Consumidor* consumidores[NUM_PRODUCTORES];
    	
    	int identificadores[NUM_PRODUCTORES];
    
    	int i;
    	//inicialisamos los productores y consumidores
    	for (i=0; i<NUM_PRODUCTORES; i++)	{		
    		productores[i] = new Productor(i);
    		consumidores[i] = new Consumidor(i); 
    	}
    
    	//comenzamos a producir y consumir
    	for (i=0; i<NUM_PRODUCTORES; i++)	{
    		productores[i]->join_thread();
    		consumidores[i]->join_thread();
    	}
    	return 0;
    }
#Explicacion
##Librerias usadas
    #include<thread>
    #include<mutex>
    #include<deque>
    #include <condition_variable>
Aparte de hilos utilizaremos mutex, condition_variable y deque.

La clase condition_variable es una primitiva de sincronización que se puede usar para bloquear un subproceso, o varios subprocesos al mismo tiempo, hasta que otro subproceso modifique una variable compartida (la condición) y notifique a condition_variable.

La clase deque es una cola de dos extremos. Las colas de dos extremos son contenedores de secuencias con tamaños dinámicos que se pueden expandir o contraer en ambos extremos (ya sea en la parte delantera o trasera).
##Clase Productor
Crearemos la clase productor, esta tendra como atributos un thread con nombre t, un integer de nombre k que sera el id del Prodcutor, como metodo solo tendra produce que sera de tipo void y no recibira nungun paramatro.
El metodo produce sera el encargado de producir elementos para el buffer y para que los consumidores consuman estos elementos.
Dentro del metodo produce tendremos integer llamado val este sera el valor limite que tendran los elementos que se produciran, tambien habra un bucle while donde produciremos los elementos y se almacenaran en el buffer, estos iran desde val asta 0, cuando val se 0 terminara el bucle while.
En el constructor de la clase productor recibira como parametro un integer y sera su id, y inicializaremos el thread con el metodo produce.

    class Productor {
    	private:
    		thread t;
    		int k;
    		void produce()
    		{
    			//numero maximo que produciran los productores e ira disminuyendo hasta 0
    			int val=100;
    			while (val)
    			{
    				unique_lock<mutex> locker(mu);
    				cond.wait(locker, []()
    				  		{ return buffer.size() < maxBufferSize; });
    				//agregamos la produccion al buffer
    				buffer.push_back(val);
    				cout << "Productor " <<k<<" produce: "<< val << endl;
    				val--;
    				locker.unlock();
    				//notificamos al otro hilo que puede continuar
    				cond.notify_one();
    			}
    		}
    	public:
    		Productor(int id) {
    			k = id;
    			t = thread(&Productor::produce, this);
    			//t.join();
    		}
    		void join_thread(){
    			t.join();
    		}
    };
##Clase Consumidor
La clase consumidor es paracida a la anterior clase, tendra como atributos un thread y un integer llamado k que sera su id.
Como unico metodo crearemos el metodo consume que sera de tipo void y no recibira parametros.
Dentro de este metodo solo tendremos un ciclo while infinito ya que los consumidores siempre estaran consumiendo del buffer, dentro de este ciclo
tendremos un valor val que sera el ultimo elemento del buffer y este sera consumido.
El constructor de la clase Consumidor solo recibira como parametro un integer que sera su id y el thread sera inicializado con el metodo consume.

    class Consumidor {
    	private:
    		thread t;
    		int k;
    		void consume() {
    			//los consumidores siempre estaran consumiendo
    
    			while (true)
    			{
    				unique_lock<mutex> locker(mu);
    				cond.wait(locker, []()
    				  		{ return buffer.size() > 0; });
    				//val es ultimo valor del buffer
    				int val = buffer.back();
    				//el consumidor consume el ultimo elemento
    				buffer.pop_back();
    				cout << "consumidor "<<k<<" consume:" << val << endl;
    				locker.unlock();
    				//notificamos a los otros hilos que pueden continuar
    				cond.notify_one();
    			}
    		}
    	public:
    		Consumidor(int id) {
    			k = id;
    			t = thread(&Consumidor::consume, this);
    			//t.join();
    		}
    		void join_thread(){
    			t.join();
    		}
    };
    
##Clase Main
E n la clase main primero crearemos la lista de los consumidores y productores,
despues inicializaremos los productores y consumidores con un ciclo for, finalmente ejecutaremos los trheads igual con un ciclo for

    int main(void)
    {
    	//creamos una lista de consumidores y productores
    	Productor* productores[NUM_PRODUCTORES];
    	Consumidor* consumidores[NUM_PRODUCTORES];
    	
    	int identificadores[NUM_PRODUCTORES];
    
    	int i;
    	//inicialisamos los productores y consumidores
    	for (i=0; i<NUM_PRODUCTORES; i++)	{		
    		productores[i] = new Productor(i);
    		consumidores[i] = new Consumidor(i); 
    	}
    
    	//comenzamos a producir y consumir
    	for (i=0; i<NUM_PRODUCTORES; i++)	{
    		productores[i]->join_thread();
    		consumidores[i]->join_thread();
    	}
    	return 0;
    }

#Creditos

https://www.geeksforgeeks.org/producer-consumer-problem-and-its-implementation-with-c/

https://en.cppreference.com/w/cpp/thread/mutex

https://www.youtube.com/watch?v=rKGq9AlfPR4&t=348s

https://www.youtube.com/watch?v=Ck5w-qNcKaw&t=44s
