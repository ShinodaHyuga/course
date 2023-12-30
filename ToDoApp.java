package es.exsample;

import android.annotation.SuppressLint;
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

    private EditText usernameEditText, todoEditText;
    private Button addTodoButton, showTodosButton, deleteTodoButton, showAllTodosButton; // Show All Todos ボタンの追加
    private ListView todoListView;
    private SQLiteDatabase db;
    private ArrayAdapter<String> todoAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        LinearLayout ll = new LinearLayout(this);
        ll.setOrientation(LinearLayout.VERTICAL);
        setContentView(ll);

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
        showAllTodosButton = new Button(this); // Show All Todos ボタンの初期化
        showAllTodosButton.setText("Show All Todos"); // Show All Todos ボタンのテキスト設定
        todoListView = new ListView(this);

        String dbPath = "data/data/" + getPackageName() + "/ToDoApp.db";
        db = SQLiteDatabase.openOrCreateDatabase(dbPath, null);
        createTables();

        todoAdapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_single_choice);
        todoListView.setAdapter(todoAdapter);
        todoListView.setChoiceMode(ListView.CHOICE_MODE_SINGLE);

        ll.addView(usernameEditText);
        ll.addView(todoEditText);
        ll.addView(addTodoButton);
        ll.addView(showTodosButton);
        ll.addView(deleteTodoButton);
        ll.addView(showAllTodosButton); // Show All Todos ボタンを追加
        ll.addView(todoListView);

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

        showAllTodosButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                showAllTodoItems();
            }
        });

        todoListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int position, long id) {
                String selectedTodo = todoAdapter.getItem(position);
                todoEditText.setText(selectedTodo);
            }
        });
    }

    private void createTables() {
        db.execSQL("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, username TEXT)");
        db.execSQL("CREATE TABLE IF NOT EXISTS todos (id INTEGER PRIMARY KEY, user_id INTEGER, todo TEXT)");
    }

    private void addTodoItem() {
        String username = usernameEditText.getText().toString();
        String todoItem = todoEditText.getText().toString();

        if (TextUtils.isEmpty(todoItem.trim())) {
            Toast.makeText(this, "ToDo item cannot be empty or whitespace.", Toast.LENGTH_SHORT).show();
            return;
        }

        long userId = getUserId(username);

        if (userId == -1) {
            userId = addUser(username);
        }

        ContentValues values = new ContentValues();
        values.put("user_id", userId);
        values.put("todo", todoItem);

        db.insert("todos", null, values);

        usernameEditText.setText("");
        todoEditText.setText("");
    }

    private void showTodoItems() {
        String username = usernameEditText.getText().toString();
        long userId = getUserId(username);

        if (userId == -1) {
            return;
        }

        Cursor cursor = db.rawQuery("SELECT todo FROM todos WHERE user_id = ?", new String[]{String.valueOf(userId)});

        todoAdapter.clear();
        while (cursor.moveToNext()) {
            @SuppressLint("Range") String todoItem = cursor.getString(cursor.getColumnIndex("todo"));

            if (!TextUtils.isEmpty(todoItem.trim())) {
                todoAdapter.add(todoItem);
            }
        }

        if (todoAdapter.getCount() == 0) {
            todoEditText.setVisibility(View.GONE);
            deleteTodoButton.setVisibility(View.GONE);
        } else {
            todoEditText.setVisibility(View.VISIBLE);
            deleteTodoButton.setVisibility(View.VISIBLE);
        }
    }

    private void deleteSelectedTodoItem() {
        String username = usernameEditText.getText().toString();
        String selectedTodo = todoEditText.getText().toString();

        if (selectedTodo.isEmpty()) {
            Toast.makeText(this, "Please select a ToDo item to delete.", Toast.LENGTH_SHORT).show();
            return;
        }

        long userId = getUserId(username);
        if (userId == -1) {
            return;
        }

        db.delete("todos", "user_id = ? AND todo = ?", new String[]{String.valueOf(userId), selectedTodo});

        showTodoItems();

        todoEditText.setText("");
    }

    private void showAllTodoItems() {
        Cursor cursor = db.rawQuery("SELECT users.username, todos.todo " +
                "FROM todos INNER JOIN users ON todos.user_id = users.id", null);

        todoAdapter.clear();
        while (cursor.moveToNext()) {
            @SuppressLint("Range") String username = cursor.getString(cursor.getColumnIndex("username"));
            @SuppressLint("Range") String todoItem = cursor.getString(cursor.getColumnIndex("todo"));

            if (!TextUtils.isEmpty(todoItem.trim())) {
                // ユーザ名と ToDo アイテムを表示するための文字列を構築
                String displayText = username + "\n" + todoItem;

                // ToDo アイテムを Adapter に追加
                todoAdapter.add(displayText);
            }
        }

        if (todoAdapter.getCount() == 0) {
            Toast.makeText(this, "No Todos found.", Toast.LENGTH_SHORT).show();
        }

        // ユーザネームとToDoアイテムを別々のEditTextに設定する
        todoListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int position, long id) {
                String selectedText = todoAdapter.getItem(position);

                // 改行を基準にユーザネームとToDoアイテムを分割
                String[] parts = selectedText.split("\n");
                if (parts.length == 2) {
                    String username = parts[0];
                    String todoItem = parts[1];
                    usernameEditText.setText(username);
                    todoEditText.setText(todoItem);
                }
            }
        });
    }



    private long getUserId(String username) {
        Cursor cursor = db.rawQuery("SELECT id FROM users WHERE username = ?", new String[]{username});
        try {
            if (cursor.moveToFirst()) {
                int columnIndex = cursor.getColumnIndex("id");
                if (columnIndex >= 0) {
                    return cursor.getLong(columnIndex);
                } else {
                    return -1;
                }
            }
            return -1;
        } finally {
            cursor.close();
        }
    }

    private long addUser(String username) {
        ContentValues values = new ContentValues();
        values.put("username", username);
        return db.insert("users", null, values);
    }
}
