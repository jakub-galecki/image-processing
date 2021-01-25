#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk-3.0/gtk/gtk.h>
#include "bmp_img.h"
#include "conversions.h"

static void chooseFile(GtkWidget *button, gpointer window) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
                                                    window,
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
        g_print("%s\n", filename); // TODO Handle file
    }
    gtk_widget_destroy(dialog);
}

static void combo_changed(GtkComboBox *widget, gpointer data) {
    GtkComboBox *combo = widget;
    if (gtk_combo_box_get_active(combo) != 0) {
        gchar *opt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
        g_print("%s\n", opt);
        g_free(opt);
    }
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Basic Image Processing");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    GtkWidget *box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box1);

    GtkWidget *selectFile = gtk_button_new_with_label("Select the BMP file");
    g_signal_connect(G_OBJECT(selectFile), "clicked", G_CALLBACK(chooseFile), window);
    gtk_box_pack_start(GTK_BOX(box1), selectFile, TRUE, TRUE, 0);

    GtkWidget *combo = gtk_combo_box_text_new();
    const char *opts[] = {"Select option", "Grayscale", "Rotate By Angle", "Rotate Right"};
    for (int i = 0; i < G_N_ELEMENTS(opts); i++) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), opts[i]);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX (combo), 0);
    g_signal_connect (combo, "changed", G_CALLBACK(combo_changed), NULL);
    gtk_box_pack_start(GTK_BOX(box1), combo, TRUE, TRUE, 0);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_box_pack_start(GTK_BOX(box1), grid, TRUE, TRUE, 0);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
