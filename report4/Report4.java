import java.io.*;

public class Report4 {
    public static void main(String[] args) {
        try {
            FileReader fr = new FileReader("Report4.java");
            BufferedReader br = new BufferedReader(fr);
            FileWriter fw = new FileWriter("Report4_output.txt");
            BufferedWriter bw = new BufferedWriter(fw);

            String s;
            int lineNumber = 1;                      // 行番号を格納する変数
            int multCommentJudg = 0;                 // 複数行のコメントであるか判定する変数
            while ((s = br.readLine()) != null) {
                String[] lineText = s.split("");     // 1行ずつ配列lineTextに格納
                boolean blockStartJudg = false;      // ブロック開始の有無を判定する変数
                int blockStartNum = 0;               // ブロック開始の位置を格納する変数
                boolean blockEndJudg = false;        // ブロック終了の有無を判定する変数
                int blockEndNum = 0;                 // ブロック終了の位置を格納する変数
                boolean singleCommentJudg = false;   // 一行のコメントであるか判定する変数

                // その行に文字があるか
                if (s.length() != 0) {
                    // コメントであるかの判定
                    for (int i = lineText.length - 1; i > 0; i--) {
                        // 一行のコメントであるかの判定
                        if (lineText[i - 1].equals("/") && lineText[i].equals("/")) {singleCommentJudg = true;}
                        // 複数行のコメントの開始であるかの判定
                        else if (lineText[i - 1].equals("/") && lineText[i].equals("*")) {multCommentJudg++;}
                        // 複数行のコメントの終了であるかの判定
                        else if (lineText[i - 1].equals("*") && lineText[i].equals("/")) {multCommentJudg--;}
                    }
                    // 文字列の最後が"{"であるか
                    if (lineText[lineText.length - 1].equals("{")) {blockStartJudg = true;}
                    // 文字列の最後が"}"であるか
                    else if (lineText[lineText.length - 1].equals("}")) {
                        blockEndJudg = true;
                        blockEndNum = lineText.length - 1;
                        for (int i = 0; i < blockEndNum; i++) {
                            // 文字列に"{"が含まれるか
                            if (lineText[i].equals("{")) {
                                blockStartJudg = true;
                                blockStartNum = i;
                                // { }の関係があるか
                                if (blockStartNum < blockEndNum) {blockEndJudg = true;}
                            }
                        }
                    }
                }
                bw.write(String.format("%04d", lineNumber) + ":");
                // その行がコメントでないときの処理
                if (!(singleCommentJudg || multCommentJudg > 0)) {
                    if (!(blockStartJudg) && !(blockEndJudg)) {bw.write(" ");}
                    else if (blockStartJudg && !(blockEndJudg)) {bw.write("+");}
                    else if (!(blockStartJudg) && blockEndJudg) {bw.write("-");}
                    else if (blockStartJudg && blockEndJudg) {bw.write("*");}
                }
                bw.write(" " + s + "\n");
                lineNumber++;
            }
            br.close();
            bw.close();
        }
        catch (IOException e) {
            System.out.println(e);
        }
    }
}
