#include <gtk/gtk.h>
#include <string.h>
static void openImage(GtkWidget *image, GtkWidget *parent_window);
static gboolean
button_openImage(GtkWidget *event_box,
                 GdkEventButton *event,
                 gpointer data)
{
  g_print("Event box clicked at coordinates %f,%f\n",
          event->x, event->y);
  GtkWidget *imageToTreat = (GtkWidget *)data;
  openImage(imageToTreat, event_box);
  // Returning TRUE means we handled the event, so the signal
  // emission should be stopped (don’t call any further callbacks
  // that may be connected). Return FALSE to continue invoking callbacks.
  return TRUE;
}

// static GtkWidget *
// create_image(char *myfile)
// {
//   GtkWidget *image;
//   GtkWidget *event_box;

//   image = gtk_image_new_from_file(myfile);

//   event_box = gtk_event_box_new();

//   gtk_container_add(GTK_CONTAINER(event_box), image);

//   g_signal_connect(G_OBJECT(event_box),
//                    "button_press_event",
//                    G_CALLBACK(button_press_callback),
//                    image);

//   return image;
// }

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
    image = gtk_image_new_from_file(filename);

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
  GtkWidget *image = NULL;
  GtkWidget *pVBox;
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Face Recognition");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);
  pVBox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), pVBox);

  button = gtk_button_new_with_label("Open an image");
  gtk_widget_set_tooltip_text(button, "Button widget");
  g_signal_connect(G_OBJECT(button), "clicked",
                   G_CALLBACK(button_openImage), (gpointer)&image);
  halign = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(halign), button);
 gtk_box_pack_start(GTK_BOX(pVBox), halign,FALSE, FALSE, 5);

  image = gtk_image_new_from_file("./sotd.jpg");
  gtk_box_pack_start(GTK_BOX(pVBox), image, FALSE, FALSE, 5);
  gtk_widget_show_all(window);

  g_signal_connect(G_OBJECT(window), "destroy",
                   G_CALLBACK(gtk_main_quit), NULL);
  gtk_main();

  return 0;
}