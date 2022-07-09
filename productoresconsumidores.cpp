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
	//inicializamos los productores y consumidores
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
