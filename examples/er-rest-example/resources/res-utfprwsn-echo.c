#include "contiki.h"

#include <string.h>
#include "contiki.h"
#include "rest-engine.h"
#include "dev/leds.h"
#include "er-coap.h"

#define TAMBUFFER 10

uint32_t buffernumber;
uint32_t buffer_lens[TAMBUFFER];
char buffercache[TAMBUFFER][REST_MAX_CHUNK_SIZE];

static void res_post_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);
static void res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset);

/* A simple actuator example. Toggles the red led */
RESOURCE(res_utfprwsn_echo,
         "title=\"UTFPRWSN Echo -> Params: payload = string, buffernumber = int\";rt=\"Control\"",
         res_get_handler,
         res_post_put_handler,
         res_post_put_handler,
         NULL);

static void le_parametro_request(void *request, void *response)
{
    const char *parametro = NULL;
    coap_packet_t *const coap_req = (coap_packet_t *)request;

    /* The query string can be retrieved by rest_get_query() or parsed for its key-value pairs. */
    // if (REST.get_query_variable(request, "buffernumber", &parametro))
    // {
    //     // PRINTF("GET: buffernumber = %d", buffernumber);
    //     buffernumber = atoi(parametro);
    // }

    if (REST.get_post_variable(request, "buffernumber", &parametro))
    {
        // PRINTF("POST: buffernumber = %d", buffernumber);
        buffernumber = atoi(parametro);
    }

    //define tamanho padrão do buffer caso não seja enviado.
    if (buffernumber <= 0)
    {
        buffernumber = 0;
    }

    if (REST.get_post_variable(request, "payload", &parametro))
    {
        if (buffernumber > REST_MAX_CHUNK_SIZE)
        {
            REST.set_response_status(response, REST.status.BAD_REQUEST);
            return;
        }
        else
        {
            memcpy((void *)buffercache[buffernumber], (void *)parametro, strlen(parametro));
            buffer_lens[buffernumber] = strlen(parametro);

        }
    }
}

static void le_parametro_get(void *request, void *response){
    const char *parametro = NULL;

    if (REST.get_query_variable(request, "buffernumber", &parametro))
    {
        // PRINTF("GET: buffernumber = %d", buffernumber);
        buffernumber = atoi(parametro);
    }

    REST.set_response_payload(response, buffercache[buffernumber], buffer_lens[buffernumber]);
}

static void
res_post_put_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size, int32_t *offset)
{
    le_parametro_request(request, response);
    // //converte o payload recebido por PUT em um pacote CoAP
    //coap_packet_t *const coap_req = (coap_packet_t *)request;
    // uint8_t buffer_ptr = 0;
    // //verifica se o payload enviado nao eh muito grande para a requisicao
    // if (coap_req->payload_len > REST_MAX_CHUNK_SIZE)
    // {
    //     //caso for muito grande, simplesmente configura a resposta como BAD_REQUEST e retorna
    //     REST.set_response_status(response, REST.status.BAD_REQUEST);
    //     return;
    // }
    // else
    // {
    //     //caso contrario, copia a mensagem ecoap_reffer criado
    //     memcpy((void *)buffer_msg, (void *)ccoap_re, coap_req->payload_len);
    //     //salva tambem o tamanho da mensagemcoap_reuso futuro)
    //     buffer_size = coap_req->payload_len;coap_re
    // }
}

static void
res_get_handler(void *request, void *response, uint8_t *buffer, uint16_t preferred_size,
                int32_t *offset)
{
    le_parametro_get(request, response);
    // uint32_t i;
    // uint8_t etag = 0;
    // //configura o tipo de conteudo da mensagem
    // REST.set_header_content_type(response, REST.type.TEXT_PLAIN);
    // //etag eh uma propriedade que eh utilizada pelos servidores de cache para saber se uma mensagem mudou
    // //duas mensagens com o mesmo valor devem ter o mesmo etag
    // for (i = 0; i < buffernumber; i++)
    // {
    //     //neste caso utilizamos um checksum simples como etag, mas o usuario pode usar o que quiser
    //     etag += buffer_msg[i];
    // }
    // REST.set_header_etag(response, (uint8_t *)&etag, 1);
    // //configura o payload a ser retornado
    // REST.set_response_payload(response, buffer_msg, buffernumber);
}
