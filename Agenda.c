#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTATOS 1000	// estabelece número máximo de contatos

typedef enum {
    TELA_SAIR = -1,
    TELA_PRINCIPAL,	//0
    TELA_CADASTRAR,	//1
    TELA_DESCADASTRAR,	//2
    TELA_LISTAR_TODOS,	//3
} Telas;

typedef struct
{
    char nome[128];	//nome completo
    char telefone[64];	//não é um número
} Contato;

typedef struct
{
    Telas menu;
    int totalContatos;
    Contato contatos[MAX_CONTATOS];
} AgendaTelefonica;

int main()
{
    AgendaTelefonica agenda;
    memset(&agenda, 0x0, sizeof(agenda));	//zera todos os bytes de agenda
   
    agenda.menu = TELA_PRINCIPAL; //menu da agenda
   
	FILE * memoria = fopen("contatos.bin", "rb");
    if (memoria == NULL)
    {
        printf("Erro ao abrir arquivo de memoria de contatos.\n");
        printf("Voce deve criar o arquivo 'contatos.bin' na primeira execucao.\n");
        return 1;
    }
    
    size_t lidos = fread(agenda.contatos, sizeof(Contato),
    			  MAX_CONTATOS, memoria);
    printf("Lidos %zd contatos\n", lidos);
    agenda.totalContatos = (int)lidos;	//faz totalContatos conter a quantidade de contatos lidos
    fclose(memoria);        
   
    while (agenda.menu != TELA_SAIR)
    {
        switch (agenda.menu)
        {
            case TELA_PRINCIPAL:
            {
                printf("-------------------------------\n");
                printf("\tAGENDA TELEFONICA: \n");
                printf("-------------------------------\n");
                int opcao = -1;
                do
                {
                    printf("Escolha uma opcao:\n");
                    printf("\tDigite 1 para cadastrar contato\n");
                    printf("\tDigite 2 para remover contato\n");
                    printf("\tDigite 3 para listar todos os contatos\n");
                    printf("\tDigite 0 para sair do programa\n");
                    scanf("%d", &opcao);
                    int c = 0; //solução empírica para esvaziamento de buffer
                    while ((c = getchar()) != '\n' && c!= EOF) {}
                } while (opcao < 0 || opcao >3);
               
                if (opcao == 0)
                    agenda.menu = TELA_SAIR;
               
                if (opcao == 1)
                {
                    agenda.menu = TELA_CADASTRAR;
                    if(agenda.totalContatos >= MAX_CONTATOS)
                    {
                        printf("\tAgenda estah lotada, nao eh possivel adicionar novos contatos.\n");
                        agenda.menu = TELA_PRINCIPAL;
                    }
                }  
               
                if (opcao == 2)
                {
                 agenda.menu = TELA_DESCADASTRAR;
                 if(agenda.totalContatos == 0)
                 {
                     printf("\tNao ha nenhum contato cadastrado para remover.\n");
                     agenda.menu = TELA_PRINCIPAL;
                 }
                }
               
                if(opcao == 3)
                {
                 agenda.menu = TELA_LISTAR_TODOS;
                 if(agenda.totalContatos == 0)
                 {
                     printf("\tAgenda nao tem nenhum contato para listar.\n");
                     agenda.menu = TELA_PRINCIPAL;
                 }
                }
            }
            break;
           
            case TELA_CADASTRAR:
                {
                	printf("-------------------------------\n");
                	printf("\tTELA DE CADASTRO:\n");
                	printf("-------------------------------\n");
                	Contato novo;
                	char opcao = 's';
                	printf("Vc deseja inserir um novo contato? Enter para continuar\n");
                int c = 0;
                while ((c = getchar()) != '\n' && c != EOF) {}
                	
                	do
					{				
                    	memset(&novo, 0x0, sizeof(novo));
                    	printf("Digite um novo nome para o contato: \n");
                	    fgets(novo.nome, 128, stdin);
                    	int id = 0;
                    	while (novo.nome[id] != '\0' && novo.nome[id] != '\n')
                    		id++;
                    	novo.nome[id] = '\0';
                    	printf("Digite um novo telefone para o contato:\n");
                    	fgets(novo.telefone, 64, stdin);
                    	id = 0;
                    	while (novo.telefone[id] != '\0' && novo.telefone[id] != '\n')
                    		id++;
                    	novo.telefone[id] = '\0';
                    	printf("Tem certeza que quer adicionar este contato? [s/n]\n");
                    	opcao = getchar();
                    
						//limpa o buffer para evitar problemas
						int c = 0;
                    	while ((c = getchar()) != '\n' && c != EOF) {}
					} while (opcao == 'n' || opcao == 'N');
					
					agenda.contatos[agenda.totalContatos] = novo;
					agenda.totalContatos++;

					memoria = fopen("contatos.bin", "w+b");
					if (memoria == NULL)
					{
						printf("erro ao abrir contatos.bin\n");
						return 0;
					}

                	size_t salvos = fwrite(agenda.contatos, sizeof (Contato), agenda.totalContatos, memoria);
					if (salvos != (size_t)agenda.totalContatos)
					{
						printf("erro ao salvar contatos na memoria contatos.bin\n");
						return 1;
					}
					fclose(memoria);
 
                	
                	agenda.menu = TELA_PRINCIPAL;
                }
                break;
               
            case TELA_DESCADASTRAR:
                {
                	printf("-------------------------------\n");
                    printf("\tTELA DE EXCLUSÃO:\n");
                    printf("-------------------------------\n");
                    int i;
                   
                    for (i=0; i<agenda.totalContatos; i++)
                    {
                    	printf("Contato %d\n", i);
                    	printf("\tNome: %s\n", agenda.contatos[i].nome);
                    	printf("\tTelefone: %s\n", agenda.contatos[i].telefone);
                    	printf("---------------------------------------------------\n");
					}
					int indice = -1;
					do
					{
						printf("Digite um indice de contato para excluir: de 0 ate 999\n");
						scanf("%d", &indice);
						
						//limpa o buffer para evitar problemas
						int c = 0;
						while ((c = getchar()) != '\n' && c != EOF) {}
					} while (indice <0 || indice >= agenda.totalContatos);

					for (i = indice; i < agenda.totalContatos-1; i++)
					agenda.contatos[i] = agenda.contatos[i+1];
					agenda.totalContatos--;

					memoria = fopen("contatos.bin", "w+b");
					if (memoria == NULL)
					{
						printf("erro ao abrir contatos.bin\n");
						return 0;
					}

					size_t salvos = fwrite(agenda.contatos, sizeof (Contato), agenda.totalContatos, memoria);
					if (salvos != (size_t)agenda.totalContatos)
					{
						printf("erro ao salvar contatos na memoria contatos.bin\n");
						return 1;
					}
					fclose(memoria);
               
                	
					agenda.menu = TELA_PRINCIPAL;
                }
                break;
           
            case TELA_LISTAR_TODOS:
                {
                	printf("-------------------------------\n");
                    printf("\tLISTANDO CONTATOS:\n");
                    printf("-------------------------------\n");
                    int i;
                    for (i = 0; i < agenda.totalContatos; i++)
                    {
                        printf("Contato %d\n", i);
                        printf("\tNome: %s\n", agenda.contatos[i].nome);
                        printf("\tTelefone: %s\n", agenda.contatos[i].telefone);
                        printf("-------------------------------\n");
                    }
                	agenda.menu = TELA_PRINCIPAL;
                }
                break;
        }
    }

return 0;
}
