public class ProductRecord {
	public ProductRecord(){
		
	}
	String Name;
	String ID;
	String Category;
	String Price;
	String CurrentStock;
	String MinStock;
	String Memo;
	String[] tokens;

	public void Reading(String line){
		if((!line.startsWith("//")) && (!line.startsWith(" "))){
			tokens = line.split("[:]");
			
			if(tokens.length == 6){
				Name = tokens[0].trim();
				ID = tokens[1].trim();
				category();
				Price = tokens[2].trim();
				CurrentStock = tokens[3].trim();
				MinStock = tokens[4].trim();
				Memo = tokens[5].trim();
				}
				else if (tokens.length == 5){
				Name = tokens[0].trim();
				ID = tokens[1].trim();
				category();
				Price = tokens[2].trim();
				CurrentStock = tokens[3].trim();
				MinStock = tokens[4].trim();
				Memo = "";
				}
				else{
				}
		}
	}
	
	public void category(){
		String[] tokens2 = ID.split("[-]");
		
		switch(Integer.parseInt(tokens2[0])){
		
		case 1:
			Category = "Food";
			break;
		case 2:
			Category = "Office";
			break;
		case 3:
			Category = "Misc";
			break;
		case 4:
			Category = "Health";
			break;
		case 5:
			Category = "Clothing";
			break;
		}
		}
	
	public void print(){
		System.out.println(String.format("%-15s %-13s %-10s %-7s %-16s %-12s %-10s",Name,ID,Category,Price,CurrentStock,MinStock,Memo));
	}
	}
