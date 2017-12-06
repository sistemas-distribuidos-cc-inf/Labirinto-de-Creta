import java.util.Scanner;

public class Server {
   static {
      System.loadLibrary("myjni"); // myjni.dll (Windows) or libmyjni.so (Unixes)
   }
   // Native method that receives a Java String and return a Java String
   private native String sayHello(String msg);
 
   public static void main(String args[]) {
        String result = new Server().sayHello("0");
        System.out.println(result);
   	Scanner scan = new Scanner(System.in);
 	while(true){
         String s = scan.next();
        result = new Server().sayHello(s);
        System.out.println(result);
      } 
    }
}
