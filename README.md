# Loop-Scheduler-POSIX-threads-
This contains the implementation of different types of Loop Schedulers. In these programs, loop schedulers are used to compute the numerical integration in parallel. 

The types of Loop Scheduler implemented here are:-
1. Sequential Loop Scheduler (The Standard way)

2. Static Loop Scheduler: A static loop scheduler is the simplest way to achieve some parallelism. This is used to make the numerical
integration problem parallel. Essentially, with T threads, each thread will do exactly T 1 of the loop iterations. So that if the loop
has 100 iterations and there are four threads, the first threads will execute loop iterations from 0 to 24, the
second will execute iterations from 25 to 49, and so on..

3. Dynamic Loop Scheduler: A dynamic loop schedulers are essentially managed by distributing ranges of indices to threads when they
request them.
The worker threads execute a code that looks like
while(!loop.done()){
  int begin, end;
  loop.getnext(&begin,&end);
  executeinnerloop(begin,end);
}
