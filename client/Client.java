import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.awt.event.KeyEvent;
import java.awt.Color;
import java.awt.GridLayout;  
import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.SwingUtilities;
import javax.swing.ImageIcon;
import javax.swing.JOptionPane;

public class Client
{
    static private boolean processa(JFrame frame, int evt)throws MalformedURLException,
    RemoteException, NotBoundException
    {
        boolean kill = false;
        Interface game = (Interface) Naming.lookup("rmi://localhost/labirinto");    
        frame.setSize(1000,550);
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        ImageIcon minotauro = new ImageIcon("img/minotauro.gif", "Minotauro");
        ImageIcon wall = new ImageIcon("img/wall.jpg", "Parede");
        ImageIcon empty = new ImageIcon("img/empty.jpg", "Vago");
        ImageIcon player = new ImageIcon("img/player.gif", "Player");
        String result;          
        if(evt == KeyEvent.VK_D)
        {
            result = game.moveRight();
        }else if(evt == KeyEvent.VK_A)
        {
            result = game.moveLeft();
        }else if(evt == KeyEvent.VK_W)
        {
            result = game.moveUp();
        }else if(evt == KeyEvent.VK_S)
        {
            result = game.moveDown();
        }else
        {
            return kill;
        }
        JPanel panel1 = new JPanel();
        panel1.setLayout(new GridLayout(11,20));
        for(char c : result.toCharArray())
        {
            if(c == 'U')
                panel1.add(new JLabel(minotauro));
            else if (c == '#')
                panel1.add(new JLabel(wall));
            else if (c == ' ')
                panel1.add(new JLabel(empty));
            else if (c == '0')
                panel1.add(new JLabel(player));
            else if (c == '-')
            {
                kill = true;
            }
        }
        panel1.setBackground(Color.WHITE);
        frame.setContentPane(panel1);
        SwingUtilities.updateComponentTreeUI(frame);
        frame.setVisible(true);
        return kill;
    }

    public static void main(String[] args)
    {       
        JFrame frame = new JFrame("Labirinto de Creta");
        try{
            processa(frame, KeyEvent.VK_W);
        }catch(MalformedURLException e){}
        catch(RemoteException e){}
        catch(NotBoundException e){}

        frame.addKeyListener(new java.awt.event.KeyAdapter()
        {
            public void keyPressed(java.awt.event.KeyEvent evt)
            {   
                try{
                    if(processa(frame, evt.getKeyCode())){
                        int result = JOptionPane.showConfirmDialog(frame,
                            "Fim de Jogo!",
                            "Confirm Quit", JOptionPane.DEFAULT_OPTION);
                        if (result == JOptionPane.YES_OPTION)
                            System.exit(0);
                    }
                }catch(MalformedURLException e){}
                catch(RemoteException e){}
                catch(NotBoundException e){}        
            }
        });
    }   
}
