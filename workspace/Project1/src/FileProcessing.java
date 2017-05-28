import java.io.File;
import java.util.Scanner;

public class FileProcessing {
	public FileProcessing(){
		count = 0;
	}
	
	int count;
	
	public void FileInput(String FileName){
		Scanner Input = null;
		int i = 0;
		{
			try {
				File file = new File(FileName);
				
				Input = new Scanner(file);
			}
			catch (Exception e){
				System.out.println("Unknown File");
			}
			
			while (Input.hasNext()){
				String line = Input.nextLine();
				if((!line.startsWith("//")) && (!line.startsWith(" "))){
					ProductList.ProductRecord[i] = new ProductRecord();
					ProductList.ProductRecord[i].Reading(line);
					if(ProductList.ProductRecord[i].tokens.length == 5 || ProductList.ProductRecord[i].tokens.length == 6){
						i++;
						count++;
					}
				}
			}
		}
	}
}
