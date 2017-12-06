import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;	

public class ServerImplements extends UnicastRemoteObject implements Interface{
   protected ServerImplements() throws RemoteException
   {
	super();
   }

   public String move(String name) throws RemoteException
   {
        String result = new ServerImplements().sayHello(name);
	return result;
    }

   static {
      System.loadLibrary("myjni"); // myjni.dll (Windows) or libmyjni.so (Unixes)
   }
   // Native method that receives a Java String and return a Java String
   private native String sayHello(String msg);
 
  // public static void main(String args[]) {
       // String result = new ServerImplements().sayHello("0");
       // System.out.println(result);
   //	Scanner scan = new Scanner(System.in);
 //	while(true){
  //       String s = scan.next();
    //    result = new ServerImplements().sayHello(s);
    //    System.out.println(result);
     // } 
 //   }
}
