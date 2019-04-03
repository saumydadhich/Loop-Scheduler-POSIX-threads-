#include <iostream>
#include <chrono>
#include <stdio.h>
#include <cstdlib>
#include <string.h>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

pthread_mutex_t mut;

struct integrate {
	int functionid, n, intensity, thread_id, granularity;
	static float sum_iteration;
	static int begin, end;
	float a, b;
	string sync;
};
float integrate::sum_iteration = 0;
int integrate::begin = 0;
int integrate::end = 0;

void *numerical_integration(void *p){
	struct integrate *inte_thread = (integrate*)p;
	int thread_id = inte_thread -> thread_id, start, finish;
	float function = 0.0, intermediate_solution = 0.0;
	while(integrate::end != inte_thread -> n){
		pthread_mutex_lock(&mut);
		integrate::begin = integrate::end;
		if (integrate::end + inte_thread -> granularity >= inte_thread -> n)
			integrate::end = inte_thread -> n;
		else
			integrate::end += inte_thread -> granularity;
		start = integrate::begin;
		finish = integrate::end;
		pthread_mutex_unlock(&mut);
		for (int i = start; i < finish; i++){
			function = (inte_thread -> a + (((float)i+0.5)*((inte_thread -> b-inte_thread -> a)/ inte_thread -> n)));
			if(inte_thread -> functionid==1){
				if (inte_thread -> sync.compare("thread") == 0 || inte_thread -> sync.compare("chunk") == 0)
		    		intermediate_solution = intermediate_solution + f1(function,inte_thread -> intensity);
				else if(inte_thread -> sync.compare("iteration") == 0){
					pthread_mutex_lock(&mut);
					integrate::sum_iteration += f1(function, inte_thread -> intensity);
					pthread_mutex_unlock(&mut);
				}
			}
	  		else if(inte_thread -> functionid==2){
	    		if (inte_thread -> sync.compare("thread") == 0 || inte_thread -> sync.compare("chunk") == 0)
		    		intermediate_solution = intermediate_solution + f2(function,inte_thread -> intensity);
	  			else if(inte_thread -> sync.compare("iteration") == 0){
					pthread_mutex_lock(&mut);
					integrate::sum_iteration += f2(function, inte_thread -> intensity);
					pthread_mutex_unlock(&mut);
				}
	  		}
	    	else if(inte_thread -> functionid==3){
	    		if (inte_thread -> sync.compare("thread") == 0 || inte_thread -> sync.compare("chunk") == 0)
		    	   intermediate_solution = intermediate_solution + f3(function,inte_thread -> intensity);
	    		else if(inte_thread -> sync.compare("iteration") == 0){
					pthread_mutex_lock(&mut);
					integrate::sum_iteration += f3(function, inte_thread -> intensity);
					pthread_mutex_unlock(&mut);
				}
	    	}
	    	else if(inte_thread -> functionid==4){
	    		if (inte_thread -> sync.compare("thread") == 0 || inte_thread -> sync.compare("chunk") == 0)
	    		   intermediate_solution = intermediate_solution + f4(function,inte_thread -> intensity);
	    		else if(inte_thread -> sync.compare("iteration") == 0){
					pthread_mutex_lock(&mut);
					integrate::sum_iteration += f4(function, inte_thread -> intensity);
					pthread_mutex_unlock(&mut);
				}
	    	}
	    }
		if (inte_thread -> sync.compare("chunk") == 0)
    	{
    	 	pthread_mutex_lock(&mut);
    	 	integrate::sum_iteration += intermediate_solution;
    	 	intermediate_solution = 0.0;
    	 	pthread_mutex_unlock(&mut);
 		}	    
	}

    if (inte_thread -> sync.compare("thread") == 0)
    {
     	pthread_mutex_lock(&mut);
     	integrate::sum_iteration += intermediate_solution;
     	pthread_mutex_unlock(&mut);
 	}
}

int main (int argc, char* argv[]) {
	int nbthreads = atoi(argv[6]);
	pthread_t thread[nbthreads];
	if (argc < 9) {
		std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync> <granularity>"<<std::endl;
		return -1;
	}

	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	struct integrate i1[nbthreads];
  	for(int i=0; i<nbthreads; i++){
		i1[i].functionid = atoi(argv[1]); 
		i1[i].a = atof(argv[2]);
		i1[i].b = atof(argv[3]);
		i1[i].n = atoi(argv[4]);
		i1[i].intensity = atoi(argv[5]);
		i1[i].sync = argv[7];
		i1[i].granularity = atoi(argv[8]);
 		i1[i].thread_id = i; 
  		pthread_create(&thread[i], NULL, numerical_integration, (void*)(i1+i));
  	}
   	float sum = 0;
  	for(int i=0; i<nbthreads; i++){
  		pthread_join(thread[i], NULL);
  	}
    float temp = (i1[0].b-i1[0].a)/ i1[0].n;
  	integrate::sum_iteration *= temp;
	cout<<integrate::sum_iteration<<endl;
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
	std::cerr<<elapsed_seconds.count()<<std::endl;

  return 0;
}
