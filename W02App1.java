package es.exsample;

import android.os.*;
import android.view.*;
import android.widget.*;
import androidx.appcompat.app.AppCompatActivity;

public class W02App1 extends AppCompatActivity {
    TextView tv;
    EditText et;
    Button[] bt = new Button[2];
    RadioButton[] rb = new RadioButton[3];
    RadioGroup rg;
    ListView lv;
    Spinner sp;
    String[] buttonName = {"Button 1", "Button 2"};
    String[] colorName = {"Red", "Green", "Blue"};
    int[] colorCode = {0xFFFF0000, 0xFF00FF00, 0xFF0000FF}; // Red, Green, Blue のARGB値
    String[] textSizes = {"8", "16", "32", "64", "128"};

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        LinearLayout ll = new LinearLayout(this);
        ll.setOrientation(LinearLayout.VERTICAL);
        setContentView(ll);

        tv = new TextView(this);
        et = new EditText(this);
        for (int i = 0; i < bt.length; i++) {
            bt[i] = new Button(this);
            bt[i].setText(buttonName[i]);
        }
        rg = new RadioGroup(this);
        for (int i = 0; i < rb.length; i++) {
            rb[i] = new RadioButton(this);
            rb[i].setText(colorName[i]);
            rg.addView(rb[i]);
        }
        lv = new ListView(this);
        sp = new Spinner(this);

        rb[0].setChecked(true);

        ArrayAdapter<String> ad = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, textSizes);
        sp.setAdapter(ad);
        sp.setSelection(textSizes.length / 2);

        ll.addView(tv);
        ll.addView(et);
        for (Button button : bt) {
            ll.addView(button);
        }
        ll.addView(rg);
        ll.addView(sp);

        // ボタン押下時の設定
        bt[0].setOnClickListener(v -> tv.setText("こんにちは"));

        bt[1].setOnClickListener(v -> {
            // EditText が初期化されていることを確認
            if (et != null) {
                tv.setText(et.getText());
            }
        });

        // ラジオボタンクリック時のリスナ登録
        rg.setOnCheckedChangeListener((group, checkedId) -> {
            RadioButton selectedRadioButton = findViewById(checkedId);

            // 選択されたラジオボタンに対応する色をテキストビューに設定
            for (int i = 0; i < rb.length; i++) {
                if (selectedRadioButton == rb[i]) {
                    tv.setTextColor(colorCode[i]);
                    break;
                }
            }
        });

        // スピナー選択時のリスナ登録
        sp.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parentView, View selectedItemView, int position, long id) {
                // 選択された文字サイズを取得してテキストビューに設定
                String selectedTextSize = textSizes[position];
                float textSize = Float.parseFloat(selectedTextSize);
                tv.setTextSize(textSize);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parentView) {
                // 何もしない
            }
        });
    }
}

