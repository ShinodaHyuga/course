import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.util.Random;

class MyThread extends Thread {
    @Override
    public void run() {
        Random rand = new Random();
        DateTimeFormatter dtf = DateTimeFormatter.ofPattern("HH:mm:ss");

        long tID = Thread.currentThread().getId();  // スレッドのIDを取得
        LocalTime startTime = LocalTime.now();  // 開始時刻を取得
        int waitTime = rand.nextInt(9) + 2;  // 待ち秒数を取得
        try {
            Thread.sleep(waitTime * 1000);
        }
        catch (InterruptedException e) {
            System.out.println(e);
        }
        LocalTime endTime = LocalTime.now();  // 終了時刻を取得
        System.out.println(
            "ID:" + String.valueOf(String.format("%3d", tID))
            + "   start " + startTime.format(dtf)
            + "   wait " + String.valueOf(String.format("%2d", waitTime)) + "sec"
            + "   end " + endTime.format(dtf)
        );
    }
}

public class Report3 {
    public static void main(String[] arg) {
        int threadQty = 30;  // スレッドの個数を格納
        MyThread[] threads = new MyThread[threadQty];
        // すべてのスレッドを作る
        for (int i = 0; i < threadQty; i++) {
            threads[i] = new MyThread();
        }
        // すべてのスレッドを開始する
        for (MyThread t:threads) {
            t.start();
        }
        // すべてのスレッドが終了するまで待機
        for (MyThread t:threads) {
            try {
                t.join();
            }
            catch (InterruptedException e) {
                System.out.println(e);
            }
        }
        System.out.println("Program end");
    }
}
