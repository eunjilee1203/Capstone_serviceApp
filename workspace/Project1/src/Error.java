
public class Error {
	
	public Error(){
		
	}
	
	
	public static void idConflicts(ProductRecord[] PR, int num) {

		for (int i = 0; i < num; i++) {
			for (int j = i + 1; j <= num; j++) {
				if(PR[i] != null && PR[j] != null)
				{
					if ((PR[i].ID).equals(PR[j].ID)) {
						PR[j] = null;
						System.out.println("ID Conflict");
					} else {
					}
				}
			}
		}
	}
	
	public static void tokensNull(ProductRecord[] PR,int num) {
		
		for (int i = 0; i < num; i++) {
			
			if(PR[i] != null)
			{
				if(PR[i].Price.equals("")){
					PR[i] = null;
			}
			}
		}
	}
	
	public static void notPriceInt(ProductRecord[] PR, int num){
		
		for(int i=0;i<num; i++){
		try{
				
				Integer.parseInt(PR[i].Price);
			}
			catch(Exception e){
				PR[i] = null;
			}
		}
	}
	
	public static void stockMinus(ProductRecord[] PR, int num)
	{

		for (int i = 0; i < num; i++)
		{
			if(PR[i] != null)
			{
				if(Integer.parseInt(PR[i].CurrentStock)<0)
				{
					PR[i] = null;
				}
				else{
					
				}
			}
		}
		
	}
}