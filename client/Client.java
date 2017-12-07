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

public class Client
{
	static private void processa(JFrame frame, int evt)throws MalformedURLException,
	RemoteException, NotBoundException
	{
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
			return;
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
				System.out.println("Fim de jogo!");
		}
		panel1.setBackground(Color.WHITE);
		frame.setContentPane(panel1);
		SwingUtilities.updateComponentTreeUI(frame);
		frame.setVisible(true);
	}

	public static void main(String[] args)
	{		
		JFrame frame = new JFrame();
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
					processa(frame, evt.getKeyCode());
				}catch(MalformedURLException e){}
				catch(RemoteException e){}
				catch(NotBoundException e){}		
			}
		});
	}	
}
