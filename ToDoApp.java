package es.exsample;

import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;

public class ToDoApp extends AppCompatActivity {

    // UIコンポーネントの宣言
    private EditText usernameEditText, todoEditText;
    private Button addTodoButton, showTodosButton, deleteTodoButton;
    private ListView todoListView;
    private SQLiteDatabase db;
    private ArrayAdapter<String> todoAdapter;

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
        deleteTodoButton.setText("Delete Selected ToDo");
        todoListView = new ListView(this);

        // データベースの初期化
        String dbPath = "data/data/" + getPackageName() + "/ToDoApp.db";
        db = SQLiteDatabase.openOrCreateDatabase(dbPath, null);
        createTables();

        // ToDoアイテムのリスト用のアダプターを初期化
        todoAdapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_single_choice);
        todoListView.setAdapter(todoAdapter);
        todoListView.setChoiceMode(ListView.CHOICE_MODE_SINGLE);

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
                deleteSelectedTodoItem();
            }
        });

        // リストアイテムが選択されたときのリスナーを設定
        todoListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int position, long id) {
                // 選択したアイテムをEditTextに表示
                String selectedTodo = todoAdapter.getItem(position);
                todoEditText.setText(selectedTodo);
            }
        });
    }

    // データベースのテーブル作成メソッド
    private void createTables() {
        // ユーザテーブルの作成
        db.execSQL("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT)");

        // ToDoアイテムテーブルの作成
        db.execSQL("CREATE TABLE IF NOT EXISTS todos (id INTEGER PRIMARY KEY, user_id INTEGER, todo TEXT)");
    }

    // ToDoアイテムの追加メソッド
    private void addTodoItem() {
        String username = usernameEditText.getText().toString();
        String todoItem = todoEditText.getText().toString();

        // 入力が空白でないことを確認
        if (TextUtils.isEmpty(todoItem.trim())) {
            Toast.makeText(this, "ToDo item cannot be empty or whitespace.", Toast.LENGTH_SHORT).show();
            return;
        }

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

    // ユーザID取得メソッド
    private long getUserId(String username) {
        Cursor cursor = db.rawQuery("SELECT id FROM users WHERE username = ?", new String[]{username});
        try {
            if (cursor.moveToFirst()) {
                int columnIndex = cursor.getColumnIndex("id");
                if (columnIndex >= 0) {
                    return cursor.getLong(columnIndex);
                } else {
                    // カラムが存在しない場合のエラーハンドリング
                    return -1;
                }
            }
            return -1;
        } finally {
            cursor.close(); // 必ずカーソルを閉じる
        }
    }

    // ユーザ追加メソッド
    private long addUser(String username) {
        ContentValues values = new ContentValues();
        values.put("username", username);

        return db.insert("users", null, values);
    }

    // ToDoアイテムの削除メソッド
    private void deleteSelectedTodoItem() {
        String username = usernameEditText.getText().toString();
        String selectedTodo = todoEditText.getText().toString();

        if (selectedTodo.isEmpty()) {
            Toast.makeText(this, "Please select a ToDo item to delete.", Toast.LENGTH_SHORT).show();
            return;
        }

        // ユーザが存在しなければ何もしない
        long userId = getUserId(username);
        if (userId == -1) {
            return;
        }

        // ToDoアイテムの削除
        db.delete("todos", "user_id = ? AND todo = ?", new String[]{String.valueOf(userId), selectedTodo});

        // 削除後の表示を更新
        showTodoItems();

        // 入力フィールドのクリア
        todoEditText.setText("");
    }

    // ToDoアイテム表示メソッド
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
        todoAdapter.clear();
        while (cursor.moveToNext()) {
            String todoItem = cursor.getString(cursor.getColumnIndex("todo"));
            todoAdapter.add(todoItem);
        }

        // ToDoアイテムが存在しない場合はEditTextとDeleteボタンを非表示にする
        if (todoAdapter.getCount() == 0) {
            todoEditText.setVisibility(View.GONE);
            deleteTodoButton.setVisibility(View.GONE);
        } else {
            todoEditText.setVisibility(View.VISIBLE);
            deleteTodoButton.setVisibility(View.VISIBLE);
        }
    }
}
