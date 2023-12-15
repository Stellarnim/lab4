#include <gtk/gtk.h>

void quit(GtkWidget *window, gpointer data)
{
    gtk_main_quit();
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *dialog, *box;
    GtkWidget *label, *message;
    gint result;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "대화 상자");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 80);
    g_signal_connect(window, "destroy", G_CALLBACK(quit), NULL);

    dialog = gtk_dialog_new_with_buttons("긴급!", GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT,
                                         "네", GTK_RESPONSE_YES,
                                         "아니오", GTK_RESPONSE_NO,
                                         NULL);

    box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new("이제 그만 주무시겠습니까?");

    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 5);
    gtk_widget_show_all(dialog);

    result = gtk_dialog_run(GTK_DIALOG(dialog));

    switch (result)
    {
    case GTK_RESPONSE_YES:
        message = gtk_label_new("네, 안녕히 주무세요.");
        break;
    case GTK_RESPONSE_NO:
        message = gtk_label_new("일찍 주무세요.");
        break;
    default:
        message = gtk_label_new("선택하지 않았습니다.");
    }

    gtk_widget_destroy(dialog);
    gtk_container_add(GTK_CONTAINER(window), message);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}