class OlEx1 {
    static int count = 10;
    int num1, num2;
    String txt;

    OlEx1(String txt, int num1) {
        count++;
        this.txt = txt;
        this.num1 = num1;      
    }
    
    OlEx1(int num1, int num2) {
        count++;
        this.num1 = num1;
        this.num2 = num2;
    }
    
    OlEx1(int num1) {
        count++;
        this.num1 = num1;
        this.num2 = count;
    }

    void getStr() {
        String output = "";
        for (int i = 0; i < num1; i++) {
            output += txt;
        }
        System.out.println(output);
    }
    
    void getInt() {
        System.out.println(num1 + num2);
    }
}

public class Report2 {
    public static void main(String[] args) {
        OlEx1 a1 = new OlEx1("abc", 3);
        a1.getStr();
        
        OlEx1 a2 = new OlEx1("hoge", 5);
        a2.getStr();

        OlEx1 a3 = new OlEx1("xX", 10);
        a3.getStr();

        OlEx1 b1 = new OlEx1(4, 5);
        b1.getInt();

        OlEx1 b2 = new OlEx1(12, 0);
        b2.getInt();
        
        OlEx1 b3 = new OlEx1(2, -8);
        b3.getInt();
        
        OlEx1 c1 = new OlEx1(5);
        c1.getInt();
        
        OlEx1 c2 = new OlEx1(0);
        c2.getInt();
        
        OlEx1 c3 = new OlEx1(0);
        c3.getInt();
    }
}
