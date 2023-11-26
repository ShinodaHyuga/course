import os
import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf

epochs = 2

# データセットをロードする
def load_fashion_mnist_data():
    fashion_mnist_data = tf.keras.datasets.fashion_mnist
    (training_images, training_labels), (test_images, test_labels) = fashion_mnist_data.load_data()
    
    # 正規化を行う（グレースケールで表現された画像であるから）
    training_images  = training_images / 255.0
    test_images = test_images / 255.0
    
    return (training_images, training_labels), (test_images, test_labels)

# ラベルとクラスクラスの対応付け
class_names = [
    "T-shirt/top", "Trouser", "Pullover", "Dress", 
    "Coat", "Sandal", "Shirt", "Sneaker", "Bag", "Ankle boot"
]

# CNNモデルを組む
def build_model():
    model = tf.keras.models.Sequential([
        tf.keras.layers.Conv2D(32, (3, 3), activation='relu', input_shape=(28, 28, 1)),
        tf.keras.layers.Conv2D(64, (3, 3), activation='relu'),
        tf.keras.layers.MaxPooling2D(pool_size=(2, 2)),
        tf.keras.layers.Dropout(0.25),
        tf.keras.layers.Flatten(),
        tf.keras.layers.Dense(128, activation='relu'),
        tf.keras.layers.Dropout(0.5),
        tf.keras.layers.Dense(10, activation='softmax')
    ])
    
    # モデルをコンパイルする
    model.compile(
        optimizer="adam",
        loss="sparse_categorical_crossentropy",
        metrics=["accuracy"])
    
    return model

# ネットワークの学習
def train_model(model, training_images, training_labels, epochs):
    history = model.fit(training_images, training_labels, epochs=epochs)
    return history

# モデルの評価
def evaluate_model(model, test_images, test_labels):
    test_loss, test_acc = model.evaluate(test_images, test_labels, verbose=1)
    print(f"Test accuracy: {100 * test_acc:5.2f}%")
    return test_acc

# ロスと精度のグラフを描画
def plot_loss_accuracy(history, epochs):
    plt.figure(figsize=(10, 4))
    plt.subplot(1, 2, 1)
    plt.plot(range(1, epochs + 1), history.history['loss'])
    plt.title('Loss')
    plt.ylabel('Loss')
    plt.xlabel('Epoch')
    plt.grid()
    
    plt.subplot(1, 2, 2)
    plt.plot(range(1, epochs + 1), history.history['accuracy'])
    plt.title('Accuracy')
    plt.ylabel('Accuracy')
    plt.xlabel('Epoch')
    plt.grid()
    
    plt.legend(['Loss', 'Accuracy'], loc="best")
    plt.savefig("./result/graph.png")

# カレントディレクトリを取得
current_directory = os.getcwd()
# "result"ディレクトリのパスを作成
result_directory = os.path.join(current_directory, "result")
# "result"ディレクトリが存在しない場合、作成する
if not os.path.exists(result_directory):
    os.mkdir(result_directory)

# データのロードとモデルの構築
(training_images, training_labels), (test_images, test_labels) = load_fashion_mnist_data()
model = build_model()

# ネットワークの学習
history = train_model(model, training_images, training_labels, epochs)

# グラフ保存
plot_loss_accuracy(history, epochs)
plt.savefig(os.path.join(result_directory, "graph.png"))

# ラベル付けを行う
def load_my_images():
    my_labels = np.array([
        0, 0, 1, 1, 0, 0, 4, 4, 2, 8,
        8, 0, 0, 1, 1, 7, 7, 3, 3, 2,
        1, 5,
    ])
    
    len_my_images = len(my_labels)
    my_images = []  # 自作の画像データを格納
    
    # 画像を読み込む
    for i in range(len_my_images):
        # ファイルの読み込み
        img_raw = tf.io.read_file(f"./images/img{i + 1}.jpg")
        # 読み込んだ画像をRGBのTensorに変換
        img_tensor = tf.image.decode_image(img_raw)
        # 画像を28×28にリサイズ
        img_final = tf.image.resize(img_tensor, [28, 28])
        # 画像をRGBからグレースケールに変換
        img_gray = tf.image.rgb_to_grayscale(img_final)
        # Tensorの形状からサイズ1の次元を削除
        img_squeeze = tf.squeeze(img_gray)
        # my_imagesリストに追加
        my_images.append(img_squeeze)
    
    my_images = np.array(my_images)
    my_images = my_images / 255.0
    
    return my_images, my_labels

my_images, my_labels = load_my_images()

# 予測を行い、結果を表示
predictions = model.predict(my_images, verbose=1)

def display_results(predictions, my_labels):
    for i in range(len(my_labels)):
        result = class_names[np.argmax(predictions[i])]
        print(f"img{i} is [{result}] (True: {class_names[my_labels[i]]})")

# 画像とグラフを表示
def plot_image(i, predictions_array, true_label, img):
    true_label, img = true_label[i], img[i]
    plt.grid(False)
    plt.xticks([])
    plt.yticks([])

    plt.imshow(img, cmap=plt.cm.binary)

    predicted_label = np.argmax(predictions_array)

    x_label = f"{class_names[predicted_label]} {np.round(np.max(predictions_array) * 100)}%"
    if predicted_label == true_label:
        color = 'blue'
    else:
        x_label += f" ({class_names[true_label]})"
        color = 'red'

    plt.xlabel(x_label, color=color)

def plot_graph(i, predictions_array, true_label):
    true_label = true_label[i]
    label_num = len(set(training_labels))
    plt.grid(False)
    plt.xticks(range(label_num))
    thisplot = plt.bar(range(label_num), predictions_array, color="#aaaaaa")
    plt.ylim([0, 1])
    predicted_label = np.argmax(predictions_array)

    thisplot[predicted_label].set_color('red')
    thisplot[true_label].set_color('blue')

def plot_all(i):
    plt.figure(figsize=(6, 3))
    plt.subplot(1, 2, 1)
    plot_image(i, predictions[i], my_labels, my_images)
    plt.subplot(1, 2, 2)
    plot_graph(i, predictions[i], my_labels)
    plt.savefig(f"./result/img{i}.png")

for i in range(len(my_labels)):
    plot_all(i)
    print([(i, class_names[i]) for i in range(10)])

my_loss, my_acc = model.evaluate(my_images, my_labels, verbose=1)
print(f"loss        : {my_loss:2.2f}")
print(f"accuracy    : {100 * my_acc:5.2f}%")
