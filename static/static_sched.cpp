#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <iostream>

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
	int functionid, n, intensity, start_iteration, end_iteration, thread_id;
	static float sum_iteration;
	float a, b, final_sum;
	string sync;
};
float integrate::sum_iteration = 0;

void *func(void *p){
	struct integrate *inte_thread = (integrate*)p;
	int thread_id = inte_thread -> thread_id;
	float function = 0.0, intermediate_solution = 0.0;
	for (int i = inte_thread -> start_iteration; i < inte_thread -> end_iteration; i++){
		//printf("value of i %d \n", i);
		function = (inte_thread -> a + (((float)i+0.5)*((inte_thread -> b-inte_thread -> a)/ inte_thread -> n)));
		if(inte_thread -> functionid==1){
			if (inte_thread -> sync.compare("thread") == 0)
	    		intermediate_solution = intermediate_solution + f1(function,inte_thread -> intensity);
			else if(inte_thread -> sync.compare("iteration") == 0){
				pthread_mutex_lock(&mut);
				integrate::sum_iteration += f1(function, inte_thread -> intensity);
				pthread_mutex_unlock(&mut);
			}
		}
  		else if(inte_thread -> functionid==2){
    		if (inte_thread -> sync.compare("thread") == 0)
	    		intermediate_solution = intermediate_solution + f2(function,inte_thread -> intensity);
  			else if(inte_thread -> sync.compare("iteration") == 0){
				pthread_mutex_lock(&mut);
				integrate::sum_iteration += f2(function, inte_thread -> intensity);
				pthread_mutex_unlock(&mut);
			}
  		}
    	else if(inte_thread -> functionid==3){
    		if (inte_thread -> sync.compare("thread") == 0)
	    	   intermediate_solution = intermediate_solution + f3(function,inte_thread -> intensity);
    		else if(inte_thread -> sync.compare("iteration") == 0){
				pthread_mutex_lock(&mut);
				integrate::sum_iteration += f3(function, inte_thread -> intensity);
				pthread_mutex_unlock(&mut);
			}
    	}
    	else if(inte_thread -> functionid==4){
    		if (inte_thread -> sync.compare("thread") == 0)
    		   intermediate_solution = intermediate_solution + f4(function,inte_thread -> intensity);
    		else if(inte_thread -> sync.compare("iteration") == 0){
				pthread_mutex_lock(&mut);
				integrate::sum_iteration += f4(function, inte_thread -> intensity);
				pthread_mutex_unlock(&mut);
			}
    	}
    }
    //inte_thread -> final_sum += intermediate_solution;
    if (inte_thread -> sync.compare("thread") == 0)
    {
     	pthread_mutex_lock(&mut);
     	integrate::sum_iteration += intermediate_solution;
     	//sum_iteration += intermediate_solution;
     	pthread_mutex_unlock(&mut);
 	}
}

int main (int argc, char* argv[]) {
	int nbthreads = atoi(argv[6]);
	pthread_t thread[nbthreads];
	if (argc < 6) {
    	std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    	return -1;
	}
	//time_t start,end;
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	//start = time(NULL);
	struct integrate i1[nbthreads];
	int begin = 0;
  	for(int i=0; i<nbthreads; i++){
		i1[i].functionid = atoi(argv[1]); 
		i1[i].a = atof(argv[2]);
		i1[i].b = atof(argv[3]);
		i1[i].n = atoi(argv[4]);
		i1[i].intensity = atoi(argv[5]);
		i1[i].sync = argv[7];
		i1[i].start_iteration = begin;
		i1[i].end_iteration = begin + (i1[i].n/nbthreads);
		if(i1[i].n % nbthreads > i)
			i1[i].end_iteration++;
		i1[i].final_sum = 0;
 		i1[i].thread_id = i; 
 		begin = i1[i].end_iteration;
  		pthread_create(&thread[i], NULL, func, (void*)(i1+i));
  	}
   	float sum = 0;
  	for(int i=0; i<nbthreads; i++){
  		pthread_join(thread[i], NULL);
  		//sum += i1[i].final_sum;
  	}
  	//sum = integrate::sum_iteration;
    float temp = (i1[0].b-i1[0].a)/ i1[0].n;
  	integrate::sum_iteration *= temp;
	cout<<integrate::sum_iteration<<endl;
    //end = time(NULL);
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    //double execution_time = difftime(end,start);
	//std::cerr<<execution_time;
	std::cerr<<elapsed_seconds.count()<<std::endl;
  	return 0;
}
