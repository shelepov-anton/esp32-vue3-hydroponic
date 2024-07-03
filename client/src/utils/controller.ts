export enum ControllerMessageType {
  ALARM = 'esp32-alarm: ',
  PH_VALUE = 'esp32-pH-value: ',
  CAPACITY_VALUE = 'esp32-capacitive-value: '
}

export const checkMessage = (message: string, type: ControllerMessageType): boolean => {
  return message.includes(type)
}

export const formatMessage = (message: string): string => {
  const messageTypes = Object.values(ControllerMessageType)
  for (const type of messageTypes) {
    if (checkMessage(message, type)) {
      return message.replace(type, '')
    }
  }
  return ''
}