
public class ExecTime {

	public static void main(String[] args) {
		long startTime = System.nanoTime();
		int a;
		a=1+2;
	
		long endTime = System.nanoTime();
		System.out.println("Execution Time = " +(endTime - startTime));
		
	}

}
