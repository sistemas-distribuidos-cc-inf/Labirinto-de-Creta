import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Interface extends Remote
{
	public String move(String name) throws RemoteException;
}
