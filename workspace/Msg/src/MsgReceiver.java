
import java.io.IOException;
import java.io.DataInputStream;
import java.net.ServerSocket;
import java.net.Socket;



public class MsgReceiver {

	static int receiverPort = 6666;
	static ServerSocket welcomeSocket = null;
	static Socket receiverSocket = null;	
	static DataInputStream inFromSender = null;
	
	
	
	public static void main(String[] args) {
		
		if ( args.length == 1 )
			receiverPort = Integer.parseInt(args[0]);
		else {			
			printHelp();
			return;
		}
		
		
		try {

			// wait for a connection request from MsgSender
			welcomeSocket = new ServerSocket(receiverPort);	
			
		}
		catch (IOException e) {
			// TODO Auto-generated catch block
			System.out.println("Receiver: Server Socket Exception");
			e.printStackTrace();
		}


		try {
			// 스트림 준비			
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		
         		
		while (true) {
			
			try {				
				// read from the socket stream
				// and print to the console				
			}
			catch (IOException e) {
				//e.printStackTrace();
			}			
			catch (ClassNotFoundException e) {			
				e.printStackTrace();
			}
			
		}
			
			
	}
	
	
	private static void printHelp() {
		System.err.println("MsgReceiver  <receiver port>");
		System.err.println("where  <receiver port> is the port number at which MsgReceiver is listening");
	}


	

}
