import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;

public class Server
{
    public static void main(String[] args) throws RemoteException,
    MalformedURLException
    {
        ServerImplements server = new ServerImplements();
        Naming.rebind("rmi://localhost/labirinto", server);
        System.out.println("Running...");
    }
}
