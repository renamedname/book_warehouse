#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROZMIAR 80
#define ZA_MALO_PAMIECI 0
char NazwaPliku[200] = "dane.txt";
FILE *ffp;
typedef struct wezel {

  char *tytul;
  char *autor;
  double cena;
  int ilosc;
  struct wezel *nastepny;
} ksiazka;

ksiazka *pierwszy = NULL;

GtkWidget *listbox;
GtkWidget *window;
ksiazka *choice;
GtkWidget *cenaw, *cenale;
GtkWidget *ilw, *ille, *autr;

void dele(int pozycja) {
  ksiazka *magazyn = pierwszy;

  if (magazyn == NULL || pozycja < 0) {

    return;
  }

  if (pozycja == 0) {
    ksiazka *temp = magazyn;
    magazyn = temp->nastepny;
    free(temp);

    return;
  }

  ksiazka *iterator = magazyn;

  for (int i = 0; iterator != NULL && i < pozycja - 1; i++) {
    iterator = iterator->nastepny;
  }

  if (iterator == NULL || iterator->nastepny == NULL) {
    
    return;
  }

  ksiazka *temp = iterator->nastepny;
  iterator->nastepny = temp->nastepny;
  free(temp);

  printf("Ksiazka nr %d usunięta\n", pozycja);
}

void search_callback(GtkWidget *entry, gpointer data) {
  const gchar *search_text = gtk_entry_get_text(GTK_ENTRY(entry));
  
  choice = NULL;
  
  GList *children, *iter;
  children = gtk_container_get_children(GTK_CONTAINER(listbox));
  for (iter = children; iter != NULL; iter = g_list_next(iter)) {
    gtk_widget_destroy(GTK_WIDGET(iter->data));
  }
  g_list_free(children);

  ksiazka *iterator = pierwszy;

  while (iterator != NULL) {
    if (strncmp(iterator->tytul, search_text, strlen(search_text)) == 0) {
    
      GtkWidget *row = gtk_list_box_row_new();
      GtkWidget *label = gtk_label_new(iterator->tytul);
      gtk_container_add(GTK_CONTAINER(row), label);
      gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
    }

    iterator = iterator->nastepny;
  }
  gtk_widget_show_all(window);
}

static void on_listbox_row_activated(GtkListBox *listbox, GtkListBoxRow *row,
                                     gpointer user_data) {
  const gchar *selected_text =
      gtk_label_get_text(GTK_LABEL(gtk_bin_get_child(GTK_BIN(row))));

  ksiazka *iterator = pierwszy;

  while (iterator != NULL) {
    if (strcmp(iterator->tytul, selected_text) == 0) {
      choice = iterator;
      break;
    }

    iterator = iterator->nastepny;
  }



  char stc[20];
  snprintf(stc, sizeof(stc), "%.2f", choice->cena);
  gtk_entry_set_text(GTK_ENTRY(cenaw), stc);
  char stcx[20];
  snprintf(stcx, sizeof(stcx), "%0d", choice->ilosc);
  gtk_entry_set_text(GTK_ENTRY(ilw), stcx);

  gchar *t = g_strdup_printf("Autor: %s", choice->autor);
  gtk_label_set_text(GTK_LABEL(autr), t);
}

static void re_cena(GtkApplication *app, gpointer user_data) {

  choice->cena = strtod(gtk_entry_get_text(GTK_ENTRY(cenaw)), NULL);

  char stc[20];
  snprintf(stc, sizeof(stc), "%.2f", choice->cena);
  gtk_entry_set_text(GTK_ENTRY(cenaw), stc);
}

static void re_il(GtkApplication *app, gpointer user_data) {

  choice->ilosc = strtod(gtk_entry_get_text(GTK_ENTRY(ilw)), NULL);
  char stcx[20];
  snprintf(stcx, sizeof(stcx), "%0d", choice->ilosc);
  gtk_entry_set_text(GTK_ENTRY(ilw), stcx);
}

void savee(GtkApplication *app, gpointer user_data) { 
  
  
  if(zapiszMagazyn()==1){

    //fclouse(ffp);
    remove(NazwaPliku);
    rename("test.txt", NazwaPliku);
    //ffp = fopen(NazwaPliku, "r");
   // wypelnijMagazyn();
    refresh();
   // ffp = fopen(NazwaPliku, "r");
    
  }


}

static void dellete(GtkApplication *app, gpointer user_data) {

  if (choice != NULL) {
    int i = 0;

 
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(listbox));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
      gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    ksiazka *iterator = pierwszy;

    while (iterator != NULL) {
      if (iterator->tytul == choice->tytul) {
        dele(i);
      } else {
       
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *label = gtk_label_new(iterator->tytul);
        gtk_container_add(GTK_CONTAINER(row), label);
        gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
      }
      i++;
      iterator = iterator->nastepny;
    }
    choice = NULL;
    gtk_widget_show_all(window);
  }
}
GtkWidget *cenawn, *autirenter, *ilwn, *tytulenter;

static void butadd(GtkApplication *app, gpointer data) {

  const gchar *cet = gtk_entry_get_text(GTK_ENTRY(cenawn));
  const gchar *aut = gtk_entry_get_text(GTK_ENTRY(autirenter));
  const gchar *ilt = gtk_entry_get_text(GTK_ENTRY(ilwn));
  const gchar *tyt = gtk_entry_get_text(GTK_ENTRY(tytulenter));

  if (strlen(tyt) != 0) {

    ksiazka *nowy = (ksiazka *)malloc(sizeof(ksiazka));
    nowy->ilosc = atoi(ilt);
    nowy->cena = atof(cet);
    nowy->tytul = (char *)malloc((strlen("tytul") + 1) * sizeof(char));
    strcpy(nowy->tytul, tyt);
    nowy->autor = (char *)malloc((strlen("autor") + 1) * sizeof(char));
    strcpy(nowy->autor, aut);
    nowy->nastepny = NULL;
  

    ksiazka *element = nowy;
    ksiazka *iterator = pierwszy;
    ksiazka *prev = NULL;

    if (element == NULL) {
      fprintf(stderr, "Za mało pamięci!\n");
      exit(ZA_MALO_PAMIECI);
    }

    element->nastepny = NULL;

    while (iterator != NULL && strcmp(iterator->tytul, element->tytul) < 0) {
      prev = iterator;
      iterator = iterator->nastepny;
    }

    if (prev == NULL) {
      element->nastepny = pierwszy;
      pierwszy = element;
    } else { 
      prev->nastepny = element;
      element->nastepny = iterator;
    }

    refresh();
  }
  gtk_widget_destroy(data);
  // gtk_main_quit();
}
static void conffff(GtkApplication *app, gpointer user_data) {
  gtk_widget_destroy(user_data);
}

void refresh() {

  gtk_container_foreach(GTK_CONTAINER(listbox), (GtkCallback)gtk_widget_destroy,
                        NULL);


  g_signal_connect(listbox, "row-activated",
                   G_CALLBACK(on_listbox_row_activated), NULL);

  ksiazka *iterator = pierwszy;

  while (iterator != NULL) {
    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *label = gtk_label_new(iterator->tytul);
    gtk_container_add(GTK_CONTAINER(row), label);
    gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
    iterator = iterator->nastepny;
  }

  gtk_widget_show_all(window);
}

static void addd(GtkApplication *app, gpointer user_data) {
  GtkWidget *new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(new_window), "Dodaj pozycje");
  gtk_window_set_default_size(GTK_WINDOW(new_window), 300, 200);

  g_signal_connect(new_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  
  GtkWidget *cenacon, *cenal, *illen, *ilcon, *ilconw, *ilosccena;
  GtkWidget *tytulcont, *tytula, *autircont, *autira, *butad;

  ilosccena = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
  gtk_container_add(GTK_CONTAINER(new_window), ilosccena);

  tytulcont = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  tytula = gtk_label_new(NULL);
  gtk_label_set_text(GTK_LABEL(tytula), "Tytul:");
  gtk_box_pack_start(GTK_BOX(tytulcont), tytula, FALSE, FALSE, 0);
  tytulenter = gtk_entry_new();
  gtk_container_add(GTK_CONTAINER(tytulcont), tytulenter);
  gtk_container_add(GTK_CONTAINER(ilosccena), tytulcont);

  autircont = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  autira = gtk_label_new(NULL);
  gtk_label_set_text(GTK_LABEL(autira), "Autor:");
  gtk_box_pack_start(GTK_BOX(autircont), autira, FALSE, FALSE, 0);
  autirenter = gtk_entry_new();
  gtk_container_add(GTK_CONTAINER(autircont), autirenter);
  gtk_container_add(GTK_CONTAINER(ilosccena), autircont);

  cenacon = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  cenal = gtk_label_new(NULL);
  gtk_label_set_text(GTK_LABEL(cenal), "Cena:");
  gtk_box_pack_start(GTK_BOX(cenacon), cenal, FALSE, FALSE, 0);
  cenawn = gtk_entry_new();
  gtk_container_add(GTK_CONTAINER(cenacon), cenawn);
  gtk_container_add(GTK_CONTAINER(ilosccena), cenacon);

  ilcon = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  illen = gtk_label_new(NULL);
  gtk_label_set_text(GTK_LABEL(illen), "ilosc:");
  gtk_box_pack_start(GTK_BOX(ilcon), illen, FALSE, FALSE, 0);
  ilwn = gtk_entry_new();
  gtk_container_add(GTK_CONTAINER(ilcon), ilwn);
  gtk_container_add(GTK_CONTAINER(ilosccena), ilcon);

  butad = gtk_button_new_with_label("Dodaj");
  g_signal_connect(butad, "clicked", G_CALLBACK(butadd), new_window);
  gtk_container_add(GTK_CONTAINER(ilosccena), butad);

  butad = gtk_button_new_with_label("Cofnij");
  g_signal_connect(butad, "clicked", G_CALLBACK(conffff), new_window);
  gtk_container_add(GTK_CONTAINER(ilosccena), butad);

 
  gtk_widget_show_all(new_window);
}

static void activate(GtkApplication *app, gpointer user_data) {

  GtkWidget *button_box, *button_biz, *comb, *buttonv;
  GtkWidget *button, *cenacont, *ilcont, *cenailosc, *buttonadd, *buttos;

  char strd[30];

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Magazyn");
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 700);

  comb = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(window), comb);

  button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
  gtk_container_add(GTK_CONTAINER(comb), button_box);
  button_biz = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
  gtk_container_add(GTK_CONTAINER(comb), button_biz);

  cenailosc = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
  gtk_container_add(GTK_CONTAINER(button_biz), cenailosc);

  autr = gtk_label_new(NULL);

  
  gtk_box_pack_start(GTK_BOX(cenailosc), autr, FALSE, FALSE, 0);

  cenacont = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  cenale = gtk_label_new(NULL);
  gtk_label_set_text(GTK_LABEL(cenale), "Cena:");
  gtk_box_pack_start(GTK_BOX(cenacont), cenale, FALSE, FALSE, 0);
  cenaw = gtk_entry_new();
  gtk_container_add(GTK_CONTAINER(cenacont), cenaw);

  g_signal_connect(cenaw, "activate", G_CALLBACK(re_cena), NULL);
  gtk_container_add(GTK_CONTAINER(cenailosc), cenacont);

  ilcont = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  ille = gtk_label_new(NULL);
  gtk_label_set_text(GTK_LABEL(ille), "ilość:");
  gtk_box_pack_start(GTK_BOX(ilcont), ille, FALSE, FALSE, 0);
  ilw = gtk_entry_new();
  gtk_container_add(GTK_CONTAINER(ilcont), ilw);

  g_signal_connect(ilw, "activate", G_CALLBACK(re_il), NULL);
  gtk_container_add(GTK_CONTAINER(cenailosc), ilcont);

  buttonv = gtk_button_new_with_label("usuń pozycje");
  g_signal_connect(buttonv, "clicked", G_CALLBACK(dellete), NULL);
  gtk_container_add(GTK_CONTAINER(cenailosc), buttonv);

  buttonadd = gtk_button_new_with_label("dodaj pozycje");
  g_signal_connect(buttonadd, "clicked", G_CALLBACK(addd), NULL);
  gtk_container_add(GTK_CONTAINER(cenailosc), buttonadd);

  buttos = gtk_button_new_with_label("zapisz");
  g_signal_connect(buttos, "clicked", G_CALLBACK(savee), NULL);
  gtk_container_add(GTK_CONTAINER(cenailosc), buttos);

  button = gtk_button_new_with_label("Exit");
  g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy),
                           window);
  gtk_container_add(GTK_CONTAINER(cenailosc), button);

  listbox = gtk_list_box_new();
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(scrolled_window), listbox);

  GtkAdjustment *vadjust =
      gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window));
  gtk_widget_add_events(listbox, GDK_SCROLL_MASK);
  gtk_widget_set_size_request(scrolled_window, 300, 600);
  ksiazka *iterator = pierwszy;

  while (iterator != NULL) {

    GtkWidget *row = gtk_list_box_row_new();
    GtkWidget *label = gtk_label_new(iterator->tytul);
    gtk_container_add(GTK_CONTAINER(row), label);
    gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);

  
    g_signal_connect(listbox, "row-activated",
                     G_CALLBACK(on_listbox_row_activated), NULL);
    iterator = iterator->nastepny;
  }

  GtkWidget *entry = gtk_entry_new();
  gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Wyszykaj...");

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_add(GTK_CONTAINER(button_box), entry);


  gtk_container_add(GTK_CONTAINER(button_box), scrolled_window);
  g_signal_connect(entry, "activate", G_CALLBACK(search_callback), NULL);

  gtk_container_add(GTK_CONTAINER(button_box), button);



  gtk_widget_show_all(window);
}

void push_back(ksiazka *el,
               ksiazka **pierwszy) { // wstaw element na koniec listy

  FILE *fp = fopen(NazwaPliku, "r");
  if (fp == NULL) {
    printf("Brak pliku z danymi\n");
  }

  // konstruujemy nowy węzeł
  ksiazka *element;
  ksiazka *iterator = *pierwszy;

  if ((element = (ksiazka *)malloc(sizeof(ksiazka))) == NULL) {
    fprintf(stderr, "Za mało pamięci!\n");
    exit(ZA_MALO_PAMIECI);
  }
  element = el;
 
  element->nastepny = NULL;

  if (iterator == NULL) { // gdy pusta lista
    *pierwszy = element;
    return;
  }

  while (iterator->nastepny != NULL) {
    iterator = iterator->nastepny;
  }

  iterator->nastepny = element;
}

void my_split(const char *str, char *tytul, char *autor, char *ilosc,
              char *cena, char del) {
  char *token;
  token = strtok((char *)str, &del);

  if (token != NULL) {
    strcpy(tytul, token);
    token = strtok(NULL, &del);
  }

  if (token != NULL) {
    strcpy(autor, token);
    token = strtok(NULL, &del);
  }

  if (token != NULL) {
    strcpy(ilosc, token);
    token = strtok(NULL, &del);
  }

  if (token != NULL) {
    strcpy(cena, token);
  }
  return;
  fprintf(stderr, "nie poprawne danne\n");
}

int zapiszMagazyn() {
  FILE *fp = fopen("test.txt", "w");
  if (fp == NULL) {
    printf("Nie można otworzyć pliku do zapisu\n");
    return 0;
  }

  ksiazka *iterator = pierwszy;
  while (iterator != NULL) {
    fprintf(fp, "%s;%s;%d;%.2f\n", iterator->tytul, iterator->autor,
      iterator->ilosc, iterator->cena);
      iterator = iterator->nastepny;
  }


  fclose(fp);

  return 1;
}

int wypelnijMagazyn(ksiazka **pierwszy) {

  FILE *fp = ffp;
  
  if (fp == NULL) {
    printf("Brak pliku z danymi\n");
    return 0;
  }
  const int max_n = 500;
  char linia[max_n], *result;
  char tytul[200];
  char autor[100];
  char ilosc[10];
  char cena[20];


  while (fgets(linia, max_n, fp) != NULL) {
    ksiazka *element;
    if ((element = (ksiazka *)malloc(sizeof(ksiazka))) == NULL) {
      fprintf(stderr, "Za mało pamięci!\n");
      exit(ZA_MALO_PAMIECI);
    }

    my_split(linia, tytul, autor, ilosc, cena, ';');

    element->tytul = (char *)malloc((strlen(tytul) + 1) * sizeof(char));
    strcpy(element->tytul, tytul);

    element->autor = (char *)malloc((strlen(autor) + 1) * sizeof(char));
    strcpy(element->autor, autor);

    element->cena = atof(cena);
    element->ilosc = atoi(ilosc);
    printf("%s\n", element->tytul);
    push_back(element, &(*pierwszy));
  }
  fclose(fp);
  return 1;
}

void wyswietlKsiazke(int pozycja, ksiazka **magazyn) {

  ksiazka *iterator = pierwszy;

  if (pozycja >= 0 && pozycja - 1 < ROZMIAR) {

    for (int i = 1; i < pozycja; i++) {
      iterator = iterator->nastepny;
    }

    printf("Ksiazka nr %d to: %s %s %u %.2f\n", pozycja, iterator->tytul,
           iterator->autor, iterator->ilosc, iterator->cena);

  } else {
    printf("W magazynie nie ma ksiazki o podanej pozycji\n");
  }
}

int main(int argc, char **argv) {

  ffp = fopen(NazwaPliku, "r");
  if (ffp == NULL) {
    printf("Brak pliku z danymi\n");
    return 1;
  }
  wypelnijMagazyn(&pierwszy);

  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}