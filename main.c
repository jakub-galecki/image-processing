#include <stdio.h>
#include <regex.h>
#include <gtk-3.0/gtk/gtk.h>
#include "bmp_img.h"
#include "conversions.h"

typedef struct {
    gpointer window;
    BMP *bmp;
    char *path;
} Data;
static void chooseFile(GtkWidget *button, gpointer data) {
    Data *d = data;
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
                                                    d->window,
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    ("_Cancel"),
                                                    GTK_RESPONSE_CANCEL,
                                                    ("_Open"),
                                                    GTK_RESPONSE_ACCEPT,
                                                    NULL);
    gint res = gtk_dialog_run(GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        BMP *bmp = read_image(filename);
        d->bmp = bmp;
        d->path = gtk_file_chooser_get_current_folder_uri(GTK_FILE_CHOOSER(dialog));
    }
    gtk_widget_destroy(dialog);
}

static void combo_changed(GtkComboBox *widget, gpointer data) {
    Data *d = data;
    GtkComboBox *combo = widget;
    if (gtk_combo_box_get_active(combo) != 0) {
        gchar *opt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
        if (strcmp(opt, "Grayscale") == 0) {
            grayscale(&d->bmp);
        } else if (strcmp(opt, "Rotate By Angle") == 0) {
            g_print("%s", "kkk");
        } else if (strcmp(opt, "Rotate Right") == 0) {
            BMP rotated;
            rotated = rotateImageRight(d->bmp);
            *d->bmp = rotated;
        }
        g_free(opt);
    }
}

static void acceptDownload(GtkWidget *button, gpointer data) {
    Data *d = data;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Dowload File",
                                                    d->window,
                                                    flags,
                                                    ("_OK"),
                                                    GTK_RESPONSE_ACCEPT,
                                                    ("_Cancel"),
                                                    GTK_RESPONSE_REJECT,
                                                    NULL);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        write_image_by_filename(d->bmp, "output.bmp");
        g_print("%s", "File written to: ");

        g_print("%s", d->path);
    } else {
        gtk_main_quit();
    }
    gtk_widget_destroy(dialog);
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
    gtk_window_set_title(GTK_WINDOW(window), "Basic Image Processing");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    Data data;
    data.window = window;
    data.path = NULL;
    data.bmp = NULL;
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box1);

    GtkWidget *selectFile = gtk_button_new_with_label("Select the BMP file");
    g_signal_connect(G_OBJECT(selectFile), "clicked", G_CALLBACK(chooseFile), &data);
    gtk_box_pack_start(GTK_BOX(box1), selectFile, TRUE, TRUE, 0);

    GtkWidget *combo = gtk_combo_box_text_new();
    const char *opts[] = {"Select option", "Grayscale", "Rotate By Angle", "Rotate Right"};
    for (int i = 0; i < G_N_ELEMENTS(opts); i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), opts[i]);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX (combo), 0);
    g_signal_connect (combo, "changed", G_CALLBACK(combo_changed), &data);
    gtk_box_pack_start(GTK_BOX(box1), combo, TRUE, TRUE, 0);

    GtkWidget *proceed = gtk_button_new_with_label("Proceed");
    g_signal_connect(G_OBJECT(proceed), "clicked", G_CALLBACK(acceptDownload), &data);
    gtk_box_pack_start(GTK_BOX(box1), proceed, TRUE, TRUE, 0);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_box_pack_start(GTK_BOX(box1), grid, TRUE, TRUE, 0);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
