## OS Project
## Producer-Consumer 
### 1.원천코드 (producer-consumer.c)를 이용해 컴파일
```C
gcc producer-consumer.c -lpthread
```
* 아무 옵션을 주지 않고 컴파일하니 동기화 함수들이 인식이 되지 않아서 -lpthread옵션을 주고 컴파일하였다.

### 2.실행 파일 실행

* 코드 실행
```C
./a.out
```

### 3. 코드 설명
```C
pthread_t prod[5],cons[5];
sem_init(&empty,0,FULL);
sem_init(&full,0,0);
sem_init(&mutex,0,1);
```
* 쓰레드, 세마포 생성

```C
for(i;i<5;i++){
	if(createrr=pthread_create(&prod[i],NULL,(void *)producer,(void *)i)){
		printf("create err\nprogram exit...\n\n");
	exit(1);
	}
}
for(i=0;i<5;i++){
	if(createrr=pthread_create(&cons[i],NULL,(void *)consumer,(void *)i)){
		printf("create err\nprogram exit...\n\n");
		exit(1);
	}
}
	for(i=0;i<5;i++){
	pthread_join(prod[i],NULL);
}
//printf("producer join");
for(i=0;i<5;i++){
	pthread_join(cons[i],NULL);
}
```
* 멀티쓰레드 생성
   * pthread헤더에 있는 pthread_create함수와 pthread_join함수를 이용하여 구현하였음
   
   
```C
sem_destroy(&mutex);
sem_destroy(&empty);
sem_destroy(&full);
```
* 멀티쓰레드 완료후 세마포 제거

```C
sem_wait(&empty);
sem_wait(&mutex);
b=stock;
b++;
stock=b;
printf("stock+ : %d\n",stock);
count_producer++;
sem_post(&mutex);
sem_post(&full);
```
* producer (void*)형 함수
   * semaphore empty, full, mutex를 이용
   * empty 참조 값이 양수면 그 값을 1감소하고 empty 세마포를 잠근다.
   * mutex역시 값을 1감소하고 잠근다.
   * stock 생산 코드를 수행하고나서 mutex값을 1증가하고 잠금을 해제한다.
   * full 참조 값이 20미만이면 그 값을 1증가하고 consumer의 full잠금을 해제한다.

```C
sem_wait(&full);
sem_wait(&mutex);
b=stock;
b--;
stock=b;
printf("%d : %d\n",(int*)n,stock);
count_consumer++;
sem_post(&mutex);
sem_post(&empty);
```
* consumer (void*)형 함수
   * semaphore empty, full, mutex를 이용
   * full 참조 값이 양수면 그 값을 1감소하고 full 세마포를 잠근다.
   * mutex는 producer와 같은방식으로 작동한다.
   * empty 참조 값이 20미만이면 그 값을 1증가하고 producer의 empty잠금을 해제한다.
   
   
## Reader-Writer
### 1. 원천코드(reader-writer.c)를 이용해 컴파일
```C
gcc reader-writer.c -lpthread
```
* producer-consumer 컴파일 할때와 마찬가지로 -lpthread옵션을 이용해 컴파일하였다.

### 2.실행 파일 실행

* 코드 실행
```
./a.out
```

### 3. 코드 설명
```C
pthread_t read[10],write[5];
	sem_init(&mutex,0,1);
	sem_init(&wrt,0,1)
```
* 초기 쓰레드, 세마포어 생성

```C
for(i=0;i<10;i++){
		if(createrr=pthread_create(&read[i],NULL,(void *)reader,(void *)i)){
			printf("create err\nprogram exit...\n\n");
			exit(1);
		}
	}
	for(i=0;i<5;i++){
		if(createrr=pthread_create(&write[i],NULL,(void *)writer,(void *)i)){
			printf("create err\nprogram exit...\n\n");
			exit(1);
		}
	}
	
	for(i=0;i<5;i++){
		pthread_join(read[i],NULL);
	}
	//printf("reader join\n");
	
	for(i=0;i<5;i++){
		pthread_join(write[i],NULL);
	}
	//printf("writer join\n");
	//printf("end stock : %d\n",stock);	
	sem_destroy(&mutex);
	sem_destroy(&wrt);
```
* 멀티쓰레드 create, join해주기

```C
sem_wait(&wrt);
	count++;
	printf("%dth writer : %d\n",(int*)n,count);
	sem_post(&wrt);
```
* writer (void *)형 함수
   * wrt의 참조 값을 생성과 동시에 1로 초기화 하였고
   * 그 값을 1감소시키며 잠구었다.
   * 풀기 전까지 쓰는 기능을 수행한다.
  
```C
sem_wait(&mutex);
	num++;
	if(num==1){
		sem_wait(&wrt);
	}
	sem_post(&mutex);
	printf("%dth reader reads: %d\n",(int*)n,count);
	sem_wait(&mutex);
	num--;
	if(num==0){
		sem_post(&wrt);
	}
	sem_post(&mutex);
```
* reader (void *)형 함수
   * writer가 쓴 값에 접근하려고 하는 첫 번째 reader가 실행이되면
   * 전역 변수 num의 값을 1증가시키고 1일때 wrt세마포어의 참조값을 1감소시키고 잠근다.
   * 읽기가 끝나고나면 num값을 1감소 시키는데 이때 num의 값이 0이 되면
   * wrt세마포어의 참조 값을 1증가시키고 잠금을 해제한다.
   * wrt의 잠금을 해제하기 전 까지는 writer가 값을 변경하는 것이 불가능하다.
