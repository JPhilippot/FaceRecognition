#include <gtk/gtk.h>
#include <string.h>

static gboolean
button_press_callback(GtkWidget *event_box,
                      GdkEventButton *event,
                      gpointer data)
{
  g_print("Event box clicked at coordinates %f,%f\n",
          event->x, event->y);

  // Returning TRUE means we handled the event, so the signal
  // emission should be stopped (don’t call any further callbacks
  // that may be connected). Return FALSE to continue invoking callbacks.
  return TRUE;
}

static GtkWidget *
create_image(char *myfile)
{
  GtkWidget *image;
  GtkWidget *event_box;

  image = gtk_image_new_from_file(myfile);

  event_box = gtk_event_box_new();

  gtk_container_add(GTK_CONTAINER(event_box), image);

  g_signal_connect(G_OBJECT(event_box),
                   "button_press_event",
                   G_CALLBACK(button_press_callback),
                   image);

  return image;
}

static void openImage(GtkWidget *image, GtkWidget *parent_window)
{
  GtkWidget *dialog;
  GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
  gint res;

  dialog = gtk_file_chooser_dialog_new("Open File",
                                       parent_window,
                                       action,
                                       ("_Cancel"),
                                       GTK_RESPONSE_CANCEL,
                                       ("_Open"),
                                       GTK_RESPONSE_ACCEPT,
                                       NULL);

  res = gtk_dialog_run(GTK_DIALOG(dialog));
  if (res == GTK_RESPONSE_ACCEPT)
  {
    char *filename;
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
    filename = gtk_file_chooser_get_filename(chooser);
    image = create_image(filename);
    g_free(filename);
  }

  gtk_widget_destroy(dialog);
}
/***                        ------                            ***/
int main(int argc, char *argv[])
{

  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *halign;
  GtkWidget *image;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Face Recognition");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);

  button = gtk_button_new_with_label("Open an image");
  gtk_widget_set_tooltip_text(button, "Button widget");
  GtkWidget *imageToTreat;
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(openImage),(imageToTreat,window), G_OBJECT(window));

  halign = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(halign), button);
  gtk_container_add(GTK_CONTAINER(window), halign);

  gtk_widget_show_all(window);

  g_signal_connect(G_OBJECT(window), "destroy",
                   G_CALLBACK(gtk_main_quit), NULL);

  gtk_main();

  return 0;
}