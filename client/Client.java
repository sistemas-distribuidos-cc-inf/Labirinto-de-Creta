import java.util.Scanner;
import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;

public class Client
{
	public static void main(String[] args) throws MalformedURLException,
		RemoteException, NotBoundException
	{
		Interface game = (Interface) Naming.lookup("rmi://localhost/labirinto");

		System.out.println(game.move("0"));

	   	Scanner scan = new Scanner(System.in);
	 	while(true){
		    	String s = scan.next();
			System.out.println(game.move(s));
		}
	}
}
