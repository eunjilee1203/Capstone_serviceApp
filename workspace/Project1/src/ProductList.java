public class ProductList {
 
	public ProductList() {
		for (int i = 0; i < 100; i++) {
			ProductRecord[i] = null;
		}
	}
 
	static ProductRecord[] ProductRecord = new ProductRecord[100];
 
	FileProcessing FileProcessing = new FileProcessing();
 
	public void displayProductList(String FileName) {
 
		FileProcessing.FileInput(FileName);
 
		Error.idConflicts(ProductRecord, FileProcessing.count);
		Error.tokensNull(ProductRecord, FileProcessing.count);
		Error.notPriceInt(ProductRecord, FileProcessing.count);
		Error.stockMinus(ProductRecord, FileProcessing.count);
 
		System.out.println(" ");
		System.out.println("Product Name    Product ID    Category   Price   Current Stock    Safe level   Memo");
		System.out.println("-----------------------------------------------------------------------------------");
 
		for (int i = 0; i < FileProcessing.count; i++) {
			if (ProductRecord[i] != null) {
				ProductRecord[i].print();
			}
		}
	}
}