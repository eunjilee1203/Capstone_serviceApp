package proj1;


class PrimeThread extends Thread {
   
   int my_id;// fields for communicating inputs //start 
   int num_step;
   int num_thread;
   int ans = 0; // for communicating result //count prime num 
     
   PrimeThread(int id, int numStep, int numThread) {
       my_id = id;
       num_step = numStep;
       num_thread = numThread;
     }
     
     
     public void run() { // overriding, must have this type
      long startTime = System.currentTimeMillis();
     int i ;
     int i_start = my_id * (num_step/num_thread);
        int i_end = i_start + (num_step/num_thread);
        
       for(i=i_start; i<i_end; i++) {
          
          if (isPrime(i)) ans++;
          
       }
       long endTime = System.currentTimeMillis();
       long timeDiff = endTime - startTime;
       
       System.out.println("Execution Time : "+timeDiff+"ms");
       
     }
     public int getAns() { return ans; }     
     private static boolean isPrime(int x) {
          int i;
          if (x<=1) return false;
          for (i=2;i<x;i++) {
            if ((x%i == 0) && (i!=x)) return false;
          }
          return true;         
        }
     
       
       
   }


class problem1 {
     private static final int NUM_END = 200000;
     private static final int NUM_THREAD = 4; 
     
     public static void main(String[] args) {
       int counter=0;
       int i;
       long startTime = System.currentTimeMillis();
       PrimeThread[] pt = new PrimeThread[NUM_THREAD];
       for (i=0;i<NUM_THREAD;i++) {
          pt[i] = new PrimeThread(i, NUM_END,NUM_THREAD);
          pt[i].start();
       }
       
       try{
          for (i =0  ; i <NUM_THREAD;i++) {
             pt[i].join();
             counter += pt[i].getAns();
          }
       }catch(InterruptedException e) {}
       
       long endTime = System.currentTimeMillis();
       long timeDiff = endTime - startTime;
       System.out.println("NUM _ THREAD : " + NUM_THREAD);
       System.out.println("Execution Time : "+timeDiff+"ms");
       System.out.println("1..."+(NUM_END-1)+" prime# counter=" + counter +"\n");
     }
   }

    
     



