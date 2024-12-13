Descrição:
Este estado ocorre quando o botão é pressionado três vezes consecutivas enquanto o LED vermelho está aceso no Modo Convencional.

Condição de Entrada:

O LDR deve indicar claridade.
O LED vermelho deve estar ligado.
O botão deve ser pressionado três vezes consecutivas (após debounce).
Ações:

Envia uma requisição HTTP para o endereço definido (http://www.google.com.br/).
Exibe na interface serial o código de resposta HTTP ou uma mensagem de erro caso a conexão Wi-Fi falhe.
Transição:
Após enviar a requisição, o sistema retorna ao fluxo normal do Modo Convencional.

