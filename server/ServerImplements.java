import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;	

public class ServerImplements extends UnicastRemoteObject implements Interface{
   protected ServerImplements() throws RemoteException
   {
	super();
   }

   public String moveUp() throws RemoteException
   {
        String result = new ServerImplements().sayHello("w");
	return result;
    }
   public String moveDown() throws RemoteException
   {
        String result = new ServerImplements().sayHello("s");
	return result;
    }
   public String moveLeft() throws RemoteException
   {
        String result = new ServerImplements().sayHello("a");
	return result;
    }
   public String moveRight() throws RemoteException
   {
        String result = new ServerImplements().sayHello("d");
	return result;
    }
   static {
      System.loadLibrary("myjni"); // myjni.dll (Windows) or libmyjni.so (Unixes)
   }
   // Native method that receives a Java String and return a Java String
   private native String sayHello(String msg);
}
