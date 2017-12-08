import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Interface extends Remote
{
    public String moveUp() throws RemoteException;
    public String moveDown() throws RemoteException;
    public String moveLeft() throws RemoteException;
    public String moveRight() throws RemoteException;
}
