#include <gtk/gtk.h>

GtkBuilder *builder;
// 계산기의 현재 입력값을 저장하는 변수
gint current_value = 0.0;

// 연산자 및 이전 값 저장 변수
gchar operator = '\0';
gint previous_value = 0.0;

// 화면 텍스트 업데이트 함수
void update_display() {
    GtkWidget *display = GTK_WIDGET(gtk_builder_get_object(builder, "display"));
    GtkWidget *predisplay = GTK_WIDGET(gtk_builder_get_object(builder, "predisplay"));
    // 현재 값을 문자열로 변환하여 화면에 표시합니다.
    gchar *display_text = g_strdup_printf("%d", current_value);
    gchar *predisplay_text = g_strdup_printf("%d", previous_value);
    gtk_entry_set_text(GTK_ENTRY(display), display_text);
    gtk_entry_set_text(GTK_ENTRY(predisplay), predisplay_text);
    g_free(display_text);
    g_free(predisplay_text);
}

// 숫자 버튼 클릭 시 호출되는 콜백 함수
void on_number_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *display = GTK_WIDGET(gtk_builder_get_object(builder, "display"));
    GtkWidget *predisplay = GTK_WIDGET(gtk_builder_get_object(builder, "predisplay"));
    const gchar *button_label = gtk_button_get_label(button);

    gchar *new_value_str = g_strdup_printf("%s%s", gtk_entry_get_text(GTK_ENTRY(display)), button_label);

    current_value = g_strtod(new_value_str, NULL);

    update_display();

    g_free(new_value_str);
}

void on_operator_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *display = GTK_WIDGET(gtk_builder_get_object(builder, "display"));
    GtkWidget *predisplay = GTK_WIDGET(gtk_builder_get_object(builder, "predisplay"));
    operator = gtk_button_get_label(button)[0];

    previous_value = current_value;
    current_value = 0.0;

    // 화면 업데이트
    update_display();
}

// 등호 버튼 클릭 시 호출되는 콜백 함수
void on_equal_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *display = GTK_WIDGET(gtk_builder_get_object(builder, "display"));
    GtkWidget *predisplay = GTK_WIDGET(gtk_builder_get_object(builder, "predisplay"));
    switch (operator) {
        case '+':
            current_value = previous_value + current_value;
            break;
        case '-':
            current_value = previous_value - current_value;
            break;
        case '*':
            current_value = previous_value * current_value;
            break;
        case '/':
            if (current_value != 0.0) {
                current_value = previous_value / current_value;
            } else {
                // 0으로 나누기를 방지하기 위해 에러 처리
                g_print("Error: Division by zero\n");
            }
            break;
    }

    // 이전 값 초기화
    previous_value = 0.0;
    operator = '\0';

    // 화면 업데이트
    update_display();
}

// C 버튼 (클리어) 클릭 시 호출되는 콜백 함수
void on_clear_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *display = GTK_WIDGET(gtk_builder_get_object(builder, "display"));
    current_value = 0.0;
    previous_value = 0.0;

    // 화면 업데이트
    update_display();
}

// CE 버튼 (현재 입력 클리어) 클릭 시 호출되는 콜백 함수
void on_clearentry_clicked(GtkButton *button, gpointer user_data) {
    GtkWidget *display = GTK_WIDGET(gtk_builder_get_object(builder, "display"));
    current_value = 0.0;

    // 화면 업데이트
    update_display();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("calculator.glade");

    gtk_builder_connect_signals(builder, NULL);
    GtkWidget *main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    GtkWidget *display = GTK_WIDGET(gtk_builder_get_object(builder, "display"));

    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(main_window);

    gtk_main();

    g_object_unref(builder);

    return 0;
}