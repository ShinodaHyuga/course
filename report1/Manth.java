import java.util.Scanner;

public class Manth{
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        
        String[] manthList = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        boolean result = true;
        
        while (!(sc.hasNext("exit"))) {
            String input = sc.nextLine();
            String[] data = input.split(" ");
            for (int i = 0; i < data.length; i++) {
                for (int j = 0; j < data[i].length(); j++) {
                    if (Character.isDigit(data[i].charAt(j))) {
                        continue;
                    }
                    else {
                        result = false;
                        break;
                    }
                }
                if (result) {
                    int num = Integer.parseInt(data[i]);
                    if (0 < num && num < 13) {
                        System.out.println(num + ": " + manthList[num - 1]);
                    }
                    else {
                        System.out.println(num + ": incorrect value!");
                    }
                }
                else {
                    System.out.println(data[i] + ": incorrect value!");
                }
            }
        }
        System.out.println("End");
    }
}
