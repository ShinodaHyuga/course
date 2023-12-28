package es.exsample;

import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import androidx.appcompat.app.AppCompatActivity;

public class ExSample extends AppCompatActivity {

    private EditText usernameEditText, todoEditText;
    private Button addTodoButton, showTodosButton, deleteTodoButton;
    private ListView todoListView;
    private SQLiteDatabase db;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // レイアウトの作成
        LinearLayout ll = new LinearLayout(this);
        ll.setOrientation(LinearLayout.VERTICAL);
        setContentView(ll);

        // UIコンポーネントの初期化
        usernameEditText = new EditText(this);
        usernameEditText.setHint("Username");
        todoEditText = new EditText(this);
        todoEditText.setHint("ToDo Item");
        addTodoButton = new Button(this);
        addTodoButton.setText("Add ToDo");
        showTodosButton = new Button(this);
        showTodosButton.setText("Show Todos");
        deleteTodoButton = new Button(this);
        deleteTodoButton.setText("Delete ToDo");
        todoListView = new ListView(this);

        // データベースの初期化
        String dbPath = "data/data/" + getPackageName() + "/ToDoApp.db";
        db = SQLiteDatabase.openOrCreateDatabase(dbPath, null);
        createTables();

        // レイアウトにUIコンポーネントを追加
        ll.addView(usernameEditText);
        ll.addView(todoEditText);
        ll.addView(addTodoButton);
        ll.addView(showTodosButton);
        ll.addView(deleteTodoButton);
        ll.addView(todoListView);

        // ボタンのクリックリスナーを設定
        addTodoButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                addTodoItem();
            }
        });

        showTodosButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                showTodoItems();
            }
        });

        deleteTodoButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                deleteTodoItem();
            }
        });
    }

    private void createTables() {
        // ユーザテーブルの作成
        db.execSQL("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT)");

        // ToDoアイテムテーブルの作成
        db.execSQL("CREATE TABLE IF NOT EXISTS todos (id INTEGER PRIMARY KEY, user_id INTEGER, todo TEXT)");
    }

    private void addTodoItem() {
        String username = usernameEditText.getText().toString();
        String todoItem = todoEditText.getText().toString();

        // ユーザが存在しなければ新規登録
        long userId = getUserId(username);

        if (userId == -1) {
            userId = addUser(username);
        }

        // ToDoアイテムの追加
        ContentValues values = new ContentValues();
        values.put("user_id", userId);
        values.put("todo", todoItem);

        db.insert("todos", null, values);

        // 入力フィールドのクリア
        usernameEditText.setText("");
        todoEditText.setText("");
    }

    private long getUserId(String username) {
        Cursor cursor = db.rawQuery("SELECT id FROM users WHERE username = ?", new String[]{username});
        if (cursor.moveToFirst()) {
            return cursor.getLong(cursor.getColumnIndex("id"));
        }
        return -1;
    }

    private long addUser(String username) {
        ContentValues values = new ContentValues();
        values.put("username", username);

        return db.insert("users", null, values);
    }

    private void deleteTodoItem() {
        String username = usernameEditText.getText().toString();
        String todoItem = todoEditText.getText().toString();

        // ユーザが存在しなければ何もしない
        long userId = getUserId(username);
        if (userId == -1) {
            return;
        }

        // ToDoアイテムの削除
        db.delete("todos", "user_id = ? AND todo = ?", new String[]{String.valueOf(userId), todoItem});

        // 削除後の表示を更新
        showTodoItems();

        // 入力フィールドのクリア
        usernameEditText.setText("");
        todoEditText.setText("");
    }

    private void showTodoItems() {
        String username = usernameEditText.getText().toString();
        long userId = getUserId(username);

        if (userId == -1) {
            // ユーザが存在しない場合の処理
            return;
        }

        // ToDoアイテムの取得
        Cursor cursor = db.rawQuery("SELECT todo FROM todos WHERE user_id = ?", new String[]{String.valueOf(userId)});

        // 結果をListViewに表示
        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1);
        while (cursor.moveToNext()) {
            String todoItem = cursor.getString(cursor.getColumnIndex("todo"));
            adapter.add(todoItem);
        }

        todoListView.setAdapter(adapter);
    }
}
