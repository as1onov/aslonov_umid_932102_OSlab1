#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
bool ready = false;

void* producer(void*) {

	pthread_mutex_lock(&lock);
	if (ready == true) {
		pthread_mutex_unlock(&lock);
	}
	
	std::cout << "Поставщик: отправляю событие" << std::endl;
	sleep(1); // выполняем действие
	
	ready = true;
	
	pthread_cond_signal(&cond1);
	pthread_mutex_unlock(&lock);


	return nullptr;
}

void* consumer(void*) {
	
	pthread_mutex_lock(&lock);
	while (ready == false) {
		pthread_cond_wait(&cond1, &lock);
	}
	ready = false;
	std::cout << "Потребитель: получено событие" << std::endl;
	pthread_mutex_unlock(&lock);

	return nullptr;
}

int main() {
    pthread_t producerThread, consumerThread;

    pthread_create(&producerThread, nullptr, producer, nullptr);
    pthread_create(&consumerThread, nullptr, consumer, nullptr);
	
    pthread_join(consumerThread, nullptr);

    return 0;
}
