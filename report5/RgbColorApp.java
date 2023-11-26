
public class RgbColorApp extends JFrame implements ActionListener {
    public static void main(String[] args) {
        new RgbColorApp();
    }
    JTextField textFieldR = new JTextField("0", 5);
    JTextField textFieldG = new JTextField("0", 5);
    JTextField textFieldB = new JTextField("0", 5);
    JLabel labelHeader = new JLabel(" 0から255までの整数を入力してください        ");
    JLabel labelR = new JLabel(" R:");
    JLabel labelG = new JLabel("  G:");
    JLabel labelB = new JLabel("  B:");
    JLabel labelColorCode = new JLabel("カラーコード：#000000");
    JLabel labelOutput = new JLabel("");
    JButton resetButton = new JButton("リセット");
    JButton exeButton = new JButton("  実行  ");

    RgbColorApp() {
        setTitle("RGB確認アプリケーション");
        getContentPane().setLayout(new FlowLayout());
        getContentPane().add(labelHeader);
        getContentPane().add(labelR);
        getContentPane().add(textFieldR);
        getContentPane().add(labelG);
        getContentPane().add(textFieldG);
        getContentPane().add(labelB);
        getContentPane().add(textFieldB);
        getContentPane().add(resetButton);
        getContentPane().add(exeButton);
        labelOutput.setPreferredSize(new Dimension(200, 100));
        labelOutput.setBackground(new Color(0, 0, 0));
        labelOutput.setOpaque(true);
        getContentPane().add(labelColorCode);
        getContentPane().add(labelOutput);
        resetButton.addActionListener(this);
        exeButton.addActionListener(this);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(300, 250);
        setVisible(true);
    }
    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == resetButton) {
            textFieldR.setText("0");
            textFieldG.setText("0");
            textFieldB.setText("0");
        }
        else if (e.getSource() == exeButton) {
            int numRdec = Integer.parseInt(textFieldR.getText());
            int numGdec = Integer.parseInt(textFieldG.getText());
            int numBdec = Integer.parseInt(textFieldB.getText());
            String numRhex = (String.format("%02x", Integer.valueOf(numRdec))).toUpperCase();
            String numGhex = (String.format("%02x", Integer.valueOf(numGdec))).toUpperCase();
            String numBhex = (String.format("%02x", Integer.valueOf(numBdec))).toUpperCase(); 
            labelColorCode.setText("カラーコード：#" + numRhex + numGhex + numBhex);
            labelOutput.setBackground(new Color(numRdec, numGdec, numBdec));
        }
    }
}
